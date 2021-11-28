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
#include <THREADS.hpp>

sf::Mutex busy_print;


extern char passer[];

int batch_load;

int NUMBER_OF_SIMULTANIOUS_ACTIVE_THREADS=30;
int total_images_converted=0;

extern std::vector<struct to_do_list_struct> to_do;
extern int to_do_index;
int wait_stat_glob=0;

int stay_active=true;
int active_loaders=0;
int loader_index=0;
int loader_signal[150000];
void LOAD_ONE_IMAGE(size_t index);
//extern bool burn;
//extern int F2;

int count_ready_global=0;

struct image_loader {
    std::string filename;
    std::string out;
    sf::Image* image;
    bool ready=false;
    bool ok=true;
    size_t index;
    sf::Thread *thread_pointer;
    std::string previous="";
};

std::vector<struct image_loader*> image_buffer;

/*
int resize_image(sf::Image *imagebuffer, std::string &filename) {
    sf::Texture texture_buffer;
    int ret=imagebuffer->loadFromFile(filename.c_str());
    texture_buffer.loadFromImage(*imagebuffer);
    sf::RenderTexture rendertexture_buffer;
    rendertexture_buffer.create(texture_buffer.getSize().x/2.0,texture_buffer.getSize().y/2.0);
    sf::Sprite sprite_buffer;
    sprite_buffer.setTexture(texture_buffer,true);
    sprite_buffer.setScale(sf::Vector2f(0.5,0.5));
    sprite_buffer.setPosition(sf::Vector2f(0.0,0.0));
    sprite_buffer.setOrigin(sf::Vector2f(0.0,0.0));
    texture_buffer.setSmooth(true);
    rendertexture_buffer.setSmooth(true);
    rendertexture_buffer.draw(sprite_buffer);
    rendertexture_buffer.display();
    *imagebuffer=rendertexture_buffer.getTexture().copyToImage();
    return ret;
}
*/

std::map<int,bool> free_threads;
sf::Mutex free_threads_mutex;
sf::Mutex passer_lock;

int wait_for_all_loaded() {
    if (image_buffer.size()==0) return 0;
    bool all_ready=1;
    int count_ready=0;
    int count_not_exist=0;
    int count_error_loading=0;
    std::map<int, bool>::iterator it;
    for (int m=0; m<loader_index; m++) {
        if (image_buffer[m]->ready) {
            count_ready++;
            if (image_buffer[m]->ok) {
            } else {
//                all_ready=-1;
//                printf("E");
//                count_error_loading++;
//                printf("\n#error loading %s %d\n",image_buffer[m]->filename.c_str(),count_error_loading);

            }
        } else {
            if (image_buffer[m]->ok) {
                if (all_ready==1) all_ready=0;
            } else {
//                static int hop=0;
//                hop++;
//                count_not_exist++;
//                all_ready=-2;
//                if (hop>50) {
//                    printf("X");
//                }
//                printf("\n#doesn't exist %s %d\n",image_buffer[m]->filename.c_str(),count_not_exist);
            }
        }
    }
//    if (count_not_exist) {
//        printf("                                              #not exist=%d",count_not_exist);
//        return 1;
//    }
//    if (count_error_loading) {
//        printf("\n#error loading=%d\n",count_error_loading);
//        return 1;
//    }
    count_ready_global=count_ready;
    wait_stat_glob=all_ready;
    return all_ready;
};

int sharp=false;
extern bool file_exists(const char * filename);
int converter_error_filenotfound=0;
int converter_error_loadingerror=0;

void LOAD_ONE_IMAGE(size_t index) {

    int first=1;
    char fn[2000];
    while (stay_active==true) {
        strcpy(fn,image_buffer[index]->filename.c_str());
        image_buffer[index]->filename=std::string() + fn;
        if (!file_exists(image_buffer[index]->filename.c_str())) {
            printf("\nindex=%d Cannot find file on disk: %s\n",index,image_buffer[index]->filename.c_str());
            image_buffer[index]->ok=false;
            converter_error_filenotfound++;
        } else {
            passer_lock.lock();
            busy_print.lock();
            printf(passer);
            passer_lock.unlock();
            printf(" Converting to: %s\n",image_buffer[index]->out.c_str());
//            fflush(stdout);
            busy_print.unlock();
/*            printf("Stat: %d  Conv/idx/2do: %d/%d/%d  T/ok/free: %d/%d/%d",
                    wait_stat_glob,
                    total_images_converted,
                    to_do_index,
                    to_do.size(),
                    image_buffer.size(),
                    count_ready_global,
                    free_threads.size()
            );
            if (converter_error_loadingerror) printf("  Err: %d/%d",converter_error_filenotfound,converter_error_loadingerror);
            printf(" Converting %s to %s\n", image_buffer[index]->out.c_str());
*/
            while (batch_load==0) sf::sleep(sf::milliseconds(10));
            if (index>=NUMBER_OF_SIMULTANIOUS_ACTIVE_THREADS) {
                while (!image_buffer[index-NUMBER_OF_SIMULTANIOUS_ACTIVE_THREADS]->ready && stay_active==true) sf::sleep(sf::milliseconds(10));
            }
            if (stay_active==true) {
                image_buffer[index]->ok=image_buffer[index]->image->loadFromFile(image_buffer[index]->filename.c_str());
//                image_buffer[index]->ok=resize_image(image_buffer[index]->image,image_buffer[index]->filename);
                if (!image_buffer[index]->ok) {
                    printf("\nindex=%d Error loading file: %s\n",index,image_buffer[index]->filename.c_str());
                    converter_error_loadingerror++;
                } else {
                    CONVERTER(  *image_buffer[index]->image,
                                (char*)image_buffer[index]->filename.c_str(),
                                (char*)image_buffer[index]->out.c_str()
                    );
                    clear_one_image_buffer(index);
                    image_buffer[index]->ready=true;
                }
            }
        }
        first=0;
        while (loader_signal[index]==0 && stay_active==true) {
                if (!sharp) sf::sleep(sf::seconds(1.0));
                else sf::sleep(sf::seconds(0.1));
        }
        loader_signal[index]=0;
        if (stay_active==true) {
        } else if (stay_active==false) {
        }
    }
}

void push_image_file(std::string filename, std::string out) {
    size_t index=image_buffer.size();
    sharp=true;

    if (loader_index>=index) {
        if (free_threads.size()>0) {
            std::map<int, bool>::iterator it;
            free_threads_mutex.lock();
            it = free_threads.begin();
            int free_thread=it->first;
            free_threads.erase(it);
            free_threads_mutex.unlock();
            if (image_buffer[free_thread]->image == NULL)
                image_buffer[free_thread]->image = new sf::Image;
            image_buffer[free_thread]->filename=filename;
            image_buffer[free_thread]->out=out;
            image_buffer[free_thread]->ready=false;
            image_buffer[free_thread]->ok=true;
            loader_signal[free_thread]=1;
            return;
        }
    }

    if (loader_index>=index) {
        struct image_loader* image_loader_struct_pointer = new image_loader;
        image_loader_struct_pointer->image = new sf::Image;
        image_loader_struct_pointer->thread_pointer = new sf::Thread(&LOAD_ONE_IMAGE,index);

        image_loader_struct_pointer->filename=filename;
        image_loader_struct_pointer->out=out;
        image_loader_struct_pointer->ready=false;
        image_loader_struct_pointer->ok=true;
        loader_signal[loader_index]=0;
        image_buffer.push_back(image_loader_struct_pointer);
        image_buffer[image_buffer.size()-1]->thread_pointer->launch();
        loader_index++;
    } else {
        if (image_buffer[loader_index]->image == NULL)
            image_buffer[loader_index]->image = new sf::Image;
        image_buffer[loader_index]->filename=filename;
        image_buffer[loader_index]->out=out;
        image_buffer[loader_index]->ready=false;
        image_buffer[loader_index]->ok=true;
        loader_signal[loader_index]=1;
        loader_index++;
    }
}


void clear_image_buffer() {
    loader_index=0;
    total_images_converted=0;
    converter_error_filenotfound=0;
    converter_error_loadingerror=0;
    sharp=false;
    batch_load=0;
//    stay_active=false;
    for (int m=0; m<image_buffer.size(); m++) {
        if (!(image_buffer[m]->image = NULL)) delete image_buffer[m]->image;
        image_buffer[m]->image = NULL;
        image_buffer[m]->filename="";
        image_buffer[m]->out="";
//        delete image_buffer[m];
    }
    free_threads.clear();
    active_loaders=0;
}

void clear_one_image_buffer(int m) {
    if (!(image_buffer[m]->image == NULL)) delete image_buffer[m]->image;
    image_buffer[m]->image = NULL;
    image_buffer[m]->filename="";
    image_buffer[m]->out="";
    image_buffer[m]->ok=false;
    image_buffer[m]->ready=false;
    loader_signal[m]=0;

    std::map<int, bool>::iterator it;
    free_threads_mutex.lock();
    it = free_threads.find(m);
    if ( it == free_threads.end() ) {
        free_threads.insert(std::make_pair(m,true));
    } else {
        it->second=true;
    }
    free_threads_mutex.unlock();
    total_images_converted++;
}
