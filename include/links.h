#ifndef LINKS_H_
#define LINKS_H_

#define NUM_SITES 15
#define MAX_LINK_LEN 50

extern const char LINKS_ARRAY[NUM_SITES][MAX_LINK_LEN];
void get_serial_hyperlink_matrix(int H[NUM_SITES][NUM_SITES]);

#endif
