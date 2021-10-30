#include <conio.h>
#include <SFML\Graphics.hpp>
#include <cassert>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <FILE_FUNCTIONS.hpp>
#include <CGI.hpp>
#include <THREADS.hpp>
#include <FFFC.hpp>
#include <iostream>
extern sf::Mutex busy_print;
extern sf::Mutex passer_lock;
std::string output_format="";
char passer[1000];

void (*CALLER)(char *,char *);
void (*CONVERTER)(sf::Image& m_image, char *, char *);

std::vector<struct to_do_list_struct> to_do;
bool dont_overwrite=false;
int to_do_index=0;

int main(int argc, char ** argv) {

    std::string param;
//    char buf[100000];
//    setvbuf(stdout, buf, _IOFBF, sizeof(buf));
//    setvbuf(stdout, buf, _IOLBF, sizeof(buf));

    std::string directory=".";
    bool recursive=false;
    bool fail=false;
    for (int n=0; n<argc; n++) {
        param=std::string() + argv[n];
        std::transform(param.begin(), param.end(),param.begin(), ::tolower);
        if (param == "-r") recursive=true;
        else if (param == "-t") {
            NUMBER_OF_SIMULTANIOUS_ACTIVE_THREADS=25;
            if (n+1<argc) {
                NUMBER_OF_SIMULTANIOUS_ACTIVE_THREADS=atoi(argv[++n]);
                printf("Threads=%d\n",NUMBER_OF_SIMULTANIOUS_ACTIVE_THREADS);
            }
        }
        else if (param == "-o") {
            dont_overwrite=true;
        }
        else if (param == "-d") {
            if (n+1<argc) {
                directory=argv[++n];
            }
            else {
                printf("No directory specified\n");
                fail=true;
            }
        }
        else if (param == "png") output_format=param;
        else if (param == "bmp") output_format=param;
        else if (param == "jpg") output_format=param;
    }

    if ( !(argc>=1) || fail || !(output_format=="jpg" || output_format=="bmp" || output_format=="png" ) ) {
        printf("Usage: %s [-r] [-t <#threads>] [-o] [-d <directory>] -f png|jpg|bmp\n",argv[0]);
        printf("    -d <directory> = directory with image(s) to convert. (default = .)\n");
        printf("    -f <format> = \n");
        printf("Options: -r = recurse directories\n");
        printf("         -t <#threads> = maximum % off threads (default = 40)\n");
        printf("         +y = don't ask. overwrite existing (default = ask)\n");
        printf("         -o = don't overwrite existing (don't ask)\n");
        printf("WORK IN PROGRESS: NOT ALL THESE OPTIONS WORK (YET)\n");

        return -1;
    }

    std::vector<struct file_list_struct> file_list;
    int ret;
    CALLER=&ACTION;
    CONVERTER=&IMAGE_TYPE;
    if (!file_exists("out")) mkdir("out");

    total_images_converted=0;

    to_do.clear();
    to_do_index=0;
    if (!recursive) ret=read_dir(directory, file_list);
    else ret=read_subdir(directory, file_list, recursive);

    int wait_stat=0;
    struct to_do_list_struct one_to_do;
    static sf::Clock clock;
    static sf::Time time=clock.getElapsedTime();
    static sf::Time timer=clock.getElapsedTime();
    while (wait_stat == 0) {
        if (clock.getElapsedTime().asMilliseconds()-time.asMilliseconds() > 2.0 ) {
            wait_stat=wait_for_all_loaded();
            passer_lock.lock();
            sprintf(passer,"Stat: %d  Conv/idx/2do: %d/%d/%d  T/ok/free: %d/%d/%d  Err: %d/%d",
                    wait_stat,
                    total_images_converted,
                    to_do_index,
                    to_do.size(),
                    image_buffer.size(),
                    count_ready_global,
                    free_threads.size(),
                    converter_error_filenotfound,converter_error_loadingerror
            );
            passer_lock.unlock();
//            printf(passer);
//            printf("\n");

            time=clock.getElapsedTime();
        }
//        printf(" Converting %s to %s\n", in.c_str(), out.c_str());
        if (to_do_index<to_do.size())
            if (clock.getElapsedTime().asMilliseconds()-timer.asMilliseconds() > 50 ) {
                if ((to_do_index-total_images_converted < NUMBER_OF_SIMULTANIOUS_ACTIVE_THREADS)) {
                    one_to_do=to_do[to_do_index];
                    push_image_file(one_to_do.filename, one_to_do.out);
    //                printf(" CONVERTING TO %s\n", out.c_str());

                    to_do_index++;
    //                sf::sleep(sf::seconds(0.005));
                } else {
    //                printf("\n");
                }
                timer=clock.getElapsedTime();
            }
    }
    printf("Stat: %d  Conv/idx/2do: %d/%d/%d  T/ok/free: %d/%d/%d",
            wait_stat,
            total_images_converted,
            to_do_index,
            to_do.size(),
            image_buffer.size(),
            count_ready_global,
            free_threads.size()
    );
    if (converter_error_loadingerror) printf("  Err: %d/%d",converter_error_filenotfound,converter_error_loadingerror);

    return 0;
}

void IMAGE_TYPE(sf::Image& m_image,char* in, char* out) {
    mkdir_p(GetBaseDir(out).c_str());
    if (!m_image.saveToFile(out)) printf("Error saving file %s\n",out);
}

void ACTION(char* directory, char* file) {
    static sf::Clock clock;
    static sf::Time time=clock.getElapsedTime();
    static sf::Time timer=clock.getElapsedTime();
    char ext[5];
    int l=strlen(file);
    if (l>4 && file[l-4]=='.') {
        bool ok=false;
        strcpy(ext,&file[l-4]);
        std::string to=file;
        to=to.substr(0,to.find_last_of("."));
        if ( strcmp(ext,".png")==0) {
            if (output_format=="jpg") {
                ok=true;
                to=to+".jpg";
            } else if (output_format=="bmp") {
                ok=true;
                to=to+".bmp";
            }
        } else if ( strcmp(ext,".bmp")==0) {
            if (output_format=="jpg") {
                ok=true;
                to=to+".jpg";
            } else if (output_format=="png") {
                ok=true;
                to=to+".png";
            }
        } else if ( strcmp(ext,".jpg")==0) {
            if (output_format=="png") {
                ok=true;
                to=to+".png";
            } else if (output_format=="bmp") {
                ok=true;
                to=to+".bmp";
            }
        }
        static int wait_stat=0;

        if (ok) {


/*
            printf("Stat: %d  Conv/idx/2do: %d/%d/%d  T/ok/free: %d/%d/%d",
                    wait_stat,
                    total_images_converted,
                    to_do_index,
                    to_do.size(),
                    image_buffer.size(),
                    count_ready_global,
                    free_threads.size()
            );
            if (converter_error_loadingerror) printf("  Err: %d/%d",converter_error_filenotfound,converter_error_loadingerror);
*/
//            printf("         \n");

            std::string in=std::string() + directory+file;
            std::string out=std::string() + "out/"+directory+to.c_str();
//            printf(" Converting %s to %s\n", in.c_str(), out.c_str());
//            fflush(stdout);
            struct to_do_list_struct one_to_do;
            one_to_do.filename=in;
            one_to_do.out=out;
            if (!dont_overwrite) {
                to_do.push_back(one_to_do);
                busy_print.lock();
                printf("PUSHING %s\r", out.c_str());
                busy_print.unlock();
            } else {
                if (!file_exists(out.c_str())) {
                    to_do.push_back(one_to_do);
                    busy_print.lock();
                    printf("PUSHING %s\r", out.c_str());
                    busy_print.unlock();
                } else {
                    busy_print.lock();
                    printf("EXISTS  %s\r", out.c_str());
                    busy_print.unlock();
                }
            }

//            if (count_ready_global < NUMBER_OF_SIMULTANIOUS_ACTIVE_THREADS) {
//            if ((image_buffer.size() < NUMBER_OF_SIMULTANIOUS_ACTIVE_THREADS) || free_threads.size()>0) {
            if (to_do_index<to_do.size())
                if (clock.getElapsedTime().asMilliseconds()-timer.asMilliseconds() > 10 ) {
                    if ((to_do_index-total_images_converted < NUMBER_OF_SIMULTANIOUS_ACTIVE_THREADS)) {
                        one_to_do=to_do[to_do_index];
                        push_image_file(one_to_do.filename, one_to_do.out);
        //                printf(" CONVERTING TO %s\n", out.c_str());

                        to_do_index++;
    //                    sf::sleep(sf::seconds(0.005));
                    } else {
        //                printf("\n");
                    }
                    timer=clock.getElapsedTime();
                }
//            }
        }
        if (clock.getElapsedTime().asMilliseconds()-time.asMilliseconds() > 250 ) {
//            printf("ONE SECOND ");
            batch_load=1;

            wait_stat=wait_for_all_loaded();

            passer_lock.lock();
            sprintf(passer,"Stat: %d  Conv/idx/2do: %d/%d/%d  T/ok/free: %d/%d/%d  Err: %d/%d",
                    wait_stat,
                    total_images_converted,
                    to_do_index,
                    to_do.size(),
                    image_buffer.size(),
                    count_ready_global,
                    free_threads.size(),
                    converter_error_filenotfound,converter_error_loadingerror
            );
            passer_lock.unlock();
//            printf(passer);
//            printf("\n");

/*
            std::string in=std::string() + directory+file;
            std::string out=std::string() + "out/"+directory+to.c_str();
            printf(" Converting %s to %s\n", in.c_str(), out.c_str());
*/
            time=clock.getElapsedTime();
        }
    }
}

int hello() {
    sf::RenderWindow sfmlWin(sf::VideoMode(600, 360), "Hello World SFML Window");
    sf::Font font;
    if (!font.loadFromFile( "fonts/impact.ttf")) return -1;
    sf::Text message("Hello, World !", font);

    while (sfmlWin.isOpen()) {
        sf::Event e;
        while (sfmlWin.pollEvent(e)) {
            switch (e.type) {
            case sf::Event::EventType::Closed:
                sfmlWin.close();
                break;
            }
        }
        sfmlWin.clear();
        sfmlWin.draw(message);
        sfmlWin.display();
    }
    return 0;
}

/*
void takes_a_function(void (*f)(void *), void *data) {
  f(data);
}

void prints_an_int(void *data) {
  // The idiom for converting a void pointer to another kind
  // of pointer.  NO NEED TO CAST.  Note this behavior is only
  // defined if the pointer data really does point to an int.
  int *i = data;
  printf("%d", *i);
}

int i = 0;
takes_a_function(prints_an_int, &i);
*/
