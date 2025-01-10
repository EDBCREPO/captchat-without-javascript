#include <nodepp/nodepp.h>

using namespace nodepp;

#include "./Controller/generator.h"
#include "./Controller/server.h"

void onMain() { process::env::init(".env");

    if( process::env::get("mode") == "generator" )
      { generator(); } else { server(); }  

}