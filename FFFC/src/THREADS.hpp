#ifndef THREADS_HPP_INCLUDED
#define THREADS_HPP_INCLUDED

extern int NUMBER_OF_SIMULTANIOUS_ACTIVE_THREADS;

void push_image_file(std::string filename, std::string out);
extern void (*CONVERTER)(sf::Image& m_image,char *, char *);
void clear_one_image_buffer(int m);
int wait_for_all_loaded();
#endif // THREADS_HPP_INCLUDED
