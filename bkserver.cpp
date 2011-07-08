/*
    bkserver.cpp

    By Stephen Holiday 2011
    http://stephenholiday.com
    (Exception, Distance Algorithm by Anders Sewerin Johansen)

    The code is under the [Apache 2.0](http://www.apache.org/licenses/LICENSE-2.0) license.

    This is a server for loading a bktree from disk and querying over http.
    
    ./bkserver [serialized tree]
    ./bkserver bkTree.dat

*/

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib> 
#include <cstring>

#include "mongoose/mongoose.h"

#include "BKTree.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <stdio.h>

BKTree<std::string> tree;
std::string t("hefy");
void *callback(enum mg_event event,
                      struct mg_connection *conn,
                      const struct mg_request_info *request_info) {
  if (event == MG_NEW_REQUEST) {
    // Echo requested URI back to the client
    mg_printf(conn, "HTTP/1.1 200 OK\r\n\r\n");
    
    char * pch;
    pch = strtok (request_info->query_string,"-");
    
    std::string edit_distance_str(pch);
    std::cout<<edit_distance_str;
    pch = strtok (NULL,"-");
    std::string search_word(pch);
    
    int edit_distance=atoi(edit_distance_str.c_str());
    
    mg_printf(conn, "Query: %s\r\n",search_word.c_str());
    
    std::vector<std::string> results;
    results=tree.find(search_word,edit_distance);
    

    std::vector<std::string>::iterator it;
    mg_printf(conn, "Results: %d\r\n",results.size());
    for ( it=results.begin() ; it < results.end(); it++ )
        mg_printf(conn,"%s\r\n",(*it).c_str());
        //std::cout << *it << std::endl;
    
    
    return 0;  // Mark as processed
  } else {
    return NULL;
  }
}

int parse_distance(const char *query) {
    //return atoi(query[1]);
}


int main(int argc, char* argv[]) { 
    std::cout<<"Loading BKTree..."<<std::endl;
    
    tree=BKTree<std::string>();
    {
        std::ifstream iff(argv[1]);
        boost::archive::text_iarchive iar(iff);
        
        iar >> tree;
    }
        
    struct mg_context *ctx;
    const char *options[] = {"listening_ports", "8080", NULL};

    std::cout << "Starting Server....";
    ctx = mg_start(&callback, NULL, options);
    getchar();  // Wait until user hits "enter"
    mg_stop(ctx);

  return 0;
}