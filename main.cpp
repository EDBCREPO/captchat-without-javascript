#include <nodepp/nodepp.h>
#include <express/http.h>
#include <jwt/jwt.h>

using namespace nodepp;

#include "./Controller/generator.h"
#include "./Controller/server.h"

void onMain() { process::env::init(".env");

    if( process::env::get("mode") == "generator" )
      { controller::generator(); } 
  else{ controller::server(); }  

}