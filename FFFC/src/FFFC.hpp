#ifndef FFFC_HPP_INCLUDED
#define FFFC_HPP_INCLUDED

void ACTION(char* directory, char* file);
int hello();
void IMAGE_TYPE(sf::Image& m_image, char* in, char* out);
extern int total_images_converted;
extern std::map<int,bool> free_threads;
extern std::vector<struct image_loader*> image_buffer;
extern int converter_error_filenotfound;
extern int converter_error_loadingerror;
extern int batch_load;
extern int count_ready_global;

#endif // FFFC_HPP_INCLUDED
