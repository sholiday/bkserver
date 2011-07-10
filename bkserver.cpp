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

static void get_qsvar(const struct mg_request_info *request_info, const char *name, char *dst, size_t dst_len) {
    const char *qs = request_info->query_string;
    mg_get_var(qs, strlen(qs == NULL ? "" : qs), name, dst, dst_len);
}

void *callback(enum mg_event event,
                      struct mg_connection *conn,
                      const struct mg_request_info *request_info) {
  if (event == MG_NEW_REQUEST) {
    // Echo requested URI back to the client
    mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n");
    
    std::string edit_distance_str;
    std::string search_word;
    
    char cb[255];

    get_qsvar(request_info, "query", cb, sizeof(cb));
    if (cb[0] != '\0') {
        search_word=std::string(cb);
    } else {
        search_word=std::string("");
    }
    
    get_qsvar(request_info, "max_distance", cb, sizeof(cb));
    if (cb[0] != '\0') {
        edit_distance_str=std::string(cb);
    } else {
        edit_distance_str=std::string("1");
    }
    
    int edit_distance=atoi(edit_distance_str.c_str());
    
    mg_printf(conn, "{\"success\":true, \"query\":\"%s\",\"max_distance\": %d, ",search_word.c_str(),edit_distance);
    
    std::vector<std::string> results;
    results=tree.find(search_word,edit_distance);
    

    std::vector<std::string>::iterator it;
    mg_printf(conn, "\"result_size\": %d, \"results\":[",results.size());
    bool first=true;
    for ( it=results.begin() ; it < results.end(); it++ ) {
        if (first) {
            mg_printf(conn,"\"%s\"",(*it).c_str());
            first=false;
        } else {
            mg_printf(conn,",\"%s\"",(*it).c_str());
        }
    }
        //std::cout << *it << std::endl;
    mg_printf(conn,"]}");
    
    return 0;  // Mark as processed
  } else {
    return NULL;
  }
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

    std::cout << "Starting Server...."<<std::endl;
    ctx = mg_start(&callback, NULL, options);
    getchar();  // Wait until user hits "enter"
    getchar();  // Wait until user hits "enter again"
    std::cout << "Stopping Server...."<<std::endl;
    mg_stop(ctx);

  return 0;
}