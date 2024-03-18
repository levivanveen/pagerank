#define NUM_SITES 15
#define MAX_LINK_LEN 50

// Get empty hyperlink matrix and fill it with values
void get_serial_hyperlink_matrix(int H[NUM_SITES][NUM_SITES]) {   

    // Site 1 - link to all other sites
    for (int i = 2; i <= 15; i++) {
        H[0][i - 1] = 1;
    }

    // Site 2 - links to sites 1,3
    H[1][0] = 1;
    H[1][2] = 1;

    // Site 3 - links to sites 1,2,5
    H[2][0] = 1;
    H[2][1] = 1;
    H[2][4] = 1;

    // Site 4 - links to site 1
    H[3][0] = 1;

    // Site 5 - links to sites 1,3
    H[4][0] = 1;
    H[4][2] = 1;

    // Site 6 - links to sites 1,3,4,5,8,10,14,15
    H[5][0] = 1;
    H[5][2] = 1;
    H[5][3] = 1;
    H[5][4] = 1;
    H[5][7] = 1;
    H[5][9] = 1;
    H[5][13] = 1;
    H[5][14] = 1;

    // Site 7 - links to sites 1, 2, 3, 4, 5, 8, 9, 11
    H[6][0] = 1;
    H[6][1] = 1;
    H[6][2] = 1;
    H[6][3] = 1;
    H[6][4] = 1;
    H[6][7] = 1;
    H[6][8] = 1;
    H[6][10] = 1;

    // Site 8 - links to sites 1, 6, 10, 14
    H[7][0] = 1;
    H[7][5] = 1;
    H[7][9] = 1;
    H[7][13] = 1;

    // Site 9 - no outgoing links

    // Site 10 - links to sites 1, 3, 4, 6, 8
    H[9][0] = 1;
    H[9][2] = 1;
    H[9][3] = 1;
    H[9][5] = 1;
    H[9][7] = 1;

    // site 11 - links to site 1
    H[10][0] = 1;

    // Site 12 - links to sites 1, 4, 9, 13
    H[11][0] = 1;
    H[11][3] = 1;
    H[11][8] = 1;
    H[11][12] = 1;
    
    // Site 13 - links to site 1
    H[12][0] = 1;

    // Site 14 - links to sites 1, 3, 8, 9
    H[13][0] = 1;
    H[13][2] = 1;
    H[13][7] = 1;
    H[13][8] = 1;

    // Site 15 - no outgoing links

    /*
    REFERENCE:
    Site 1 links to sites 2, . . . , 15. 
    Site 2 links to sites 1, 3. 
    Site 3 links to sites 1, 2, 5. 
    Site 4 links to site 1. 
    Site 5 links to sites 1, 3. 
    Site 6 links to sites 1, 3, 4, 5, 8, 10, 14, 15. 
    Site 7 links to sites 1, 2, 3, 4, 5, 9, 11. 
    Site 8 links to sites 1, 6, 10, 14. 
    Site 10 links to sites 1, 3, 4, 6, 8. 
    Site 11 links to site 1. 
    Site 12 links to sites 1, 4, 9, 13. 
    Site 13 links to site 1.
    Site 14 links to sites 1, 3, 8, 9. 
    */
}

const char LINKS_ARRAY[NUM_SITES][MAX_LINK_LEN] = {
    "www.wlu.ca",
    "webaccess.wlu.ca",
    "library.wlu.ca",
    "www.laurierathletics.com",
    "www.wlubookstore.com",
    "www.wlusu.com",
    "www.wlufa.ca",
    "www.wlusp.com",
    "www.wlupress.wlu.ca",
    "www.mylaurier.ca",
    "www.laurierconferences.ca",
    "www.laurieralumni.ca",
    "invest.wlu.ca",
    "www.wlugsa.ca",
    "www.lauriercc.ca"
};


/* 1 Wilfrid Laurier University www.wlu.ca
2 Laurier WebAccess webaccess.wlu.ca
3 Laurier Library library.wlu.ca
4 Wilfrid Laurier University Athletics www.laurierathletics.com
5 Laurier Bookstore www.wlubookstore.com
6 Wilfrid Laurier Student Union www.wlusu.com
7 WLU Faculty Association www.wlufa.ca
8 WLU Student Publications www.wlusp.com
9 Wilfrid Laurier University Press www.wlupress.wlu.ca
10 WLU Student Portal www.mylaurier.ca
11 WLU Conference Services www.laurierconferences.ca
12 Laurier Alumni www.laurieralumni.ca
13 Investment Simulations and Competitions invest.wlu.ca
14 WLU Graduate Students Association www.wlugsa.ca
15 WLU Career and Co-op www.lauriercc.ca */