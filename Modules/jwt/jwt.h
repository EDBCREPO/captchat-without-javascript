/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_JWT
#define NODEPP_JWT

/*────────────────────────────────────────────────────────────────────────────*/

#include <nodepp/nodepp.h>
#include <nodepp/encoder.h>
#include <nodepp/crypto.h>
#include <nodepp/json.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace jwt { namespace XOR {

    bool verify ( const string_t& token, const string_t& secret ){
        try { if( token.empty()  ){ return false; }
              if( secret.empty() ){ return false; }

        auto data = regex::split( token, '.' );
        if ( data.size() != 3 ){ return false; }

        auto obj = json::parse( encoder::base64::set( data[0] ) );
        if( !obj["alg"].has_value() || obj["alg"].as<string_t>() != "XOR" )
          { return false; } 

        string_t _token = string::format("%s.%s",data[0].get(),data[1].get());
        auto sig = crypto::hmac::SHA256( secret ); sig.update( _token );
        auto ver = encoder::base64::get( sig.get() );

        return ver == data[2];

        } catch(...) { return false; }
    }

    string_t encode ( const string_t& payload, string_t secret ){

        string_t header = R"({"alg":"XOR","typ":"JWT"})";
        string_t token  = string::format("%s.%s",
            encoder::base64::get(  header ).get(),
            encoder::base64::get( payload ).get()
        );

        auto sig = crypto::hmac::SHA1( secret );
             sig.update( token );
        auto data= sig.get();

        return string::format("%s.%s.%s",
            encoder::base64::get(  header.get() ).get(),
            encoder::base64::get( payload.get() ).get(),
            encoder::base64::get(    data.get() ).get()
        );

    }

    string_t decode ( const string_t& token ){
        if( token.empty() ){ return nullptr; }

        auto data = regex::split( token, '.' );
        if ( data.size() != 3 )
           { process::error("invalid token"); }

        return encoder::base64::set( data[1] );

    }

}}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace jwt { namespace HS256 {

    bool verify ( const string_t& token, const string_t& secret ){
        try { if( token.empty()  ){ return false; }
              if( secret.empty() ){ return false; }

        auto data = regex::split( token, '.' );
        if ( data.size() != 3 ){ return false; }

        auto obj = json::parse( encoder::base64::set( data[0] ) );
        if( !obj["alg"].has_value() || obj["alg"].as<string_t>() != "HS256" )
          { return false; } 

        string_t _token = string::format("%s.%s",data[0].get(),data[1].get());
        auto sig = crypto::hmac::SHA256( secret ); sig.update( _token );
        auto ver = encoder::base64::get( sig.get() );

        return ver == data[2];

        } catch(...) { return false; }
    }

    string_t encode ( const string_t& payload, string_t secret ){

        string_t header = R"({"alg":"HS256","typ":"JWT"})";
        string_t token  = string::format("%s.%s",
            encoder::base64::get(  header ).get(),
            encoder::base64::get( payload ).get()
        );

        auto sig = crypto::hmac::SHA256( secret );
             sig.update( token );
        auto data= sig.get();

        return string::format("%s.%s.%s",
            encoder::base64::get(  header.get() ).get(),
            encoder::base64::get( payload.get() ).get(),
            encoder::base64::get(    data.get() ).get()
        );

    }

    string_t decode ( const string_t& token ){ return XOR::decode( token ); }

}}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace jwt { namespace HS384 {

    bool verify ( const string_t& token, const string_t& secret ){
        try { if( token.empty()  ){ return false; }
              if( secret.empty() ){ return false; }

        auto data = regex::split( token, '.' );
        if ( data.size() != 3 ){ return false; }

        auto obj = json::parse( encoder::base64::set( data[0] ) );
        if( !obj["alg"].has_value() || obj["alg"].as<string_t>() != "HS384" )
          { return false; } 

        string_t _token = string::format("%s.%s",data[0].get(),data[1].get());
        auto sig = crypto::hmac::SHA384( secret ); sig.update( _token );
        auto ver = encoder::base64::get( sig.get() );

        return ver == data[2];

        } catch(...) { return false; }
    }

    string_t encode ( const string_t& payload, string_t secret ){

        string_t header = R"({"alg":"HS384","typ":"JWT"})";
        string_t token  = string::format("%s.%s",
            encoder::base64::get(  header ).get(),
            encoder::base64::get( payload ).get()
        );

        auto sig = crypto::hmac::SHA384( secret );
             sig.update( token );
        auto data= sig.get();

        return string::format("%s.%s.%s",
            encoder::base64::get(  header.get() ).get(),
            encoder::base64::get( payload.get() ).get(),
            encoder::base64::get(    data.get() ).get()
        );

    }

    string_t decode ( const string_t& token ){ return XOR::decode( token ); }

}}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace jwt { namespace HS512 {

    bool verify ( const string_t& token, const string_t& secret ){
        try { if( token.empty()  ){ return false; }
              if( secret.empty() ){ return false; }

        auto data = regex::split( token, '.' );
        if ( data.size() != 3 ){ return false; }

        auto obj = json::parse( encoder::base64::set( data[0] ) );
        if( !obj["alg"].has_value() || obj["alg"].as<string_t>() != "HS512" )
          { return false; } 

        string_t _token = string::format("%s.%s",data[0].get(),data[1].get());
        auto sig = crypto::hmac::SHA512( secret ); sig.update( _token );
        auto ver = encoder::base64::get( sig.get() );

        return ver == data[2];

        } catch(...) { return false; }
    }

    string_t encode ( const string_t& payload, string_t secret ){

        string_t header = R"({"alg":"HS512","typ":"JWT"})";
        string_t token  = string::format("%s.%s",
            encoder::base64::get(  header ).get(),
            encoder::base64::get( payload ).get()
        );

        auto sig = crypto::hmac::SHA512( secret );
             sig.update( token );
        auto data= sig.get();

        return string::format("%s.%s.%s",
            encoder::base64::get(  header.get() ).get(),
            encoder::base64::get( payload.get() ).get(),
            encoder::base64::get(    data.get() ).get()
        );

    }

    string_t decode ( const string_t& token ){ return XOR::decode( token ); }

}}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace jwt {

    string_t decode ( const string_t& token ){ return XOR::decode( token ); }

    bool verify ( const string_t& token, string_t secret ){ try {

        auto data = regex::match( token, "[^.]+" ); 
        if ( data == nullptr ){ throw; } 

        auto raw = json::parse( encoder::base64::set(data) );
        auto type= raw["alg"].as<string_t>();

          if( type=="XOR"   ){ return XOR  ::verify( token, secret ); }
        elif( type=="HS256" ){ return HS256::verify( token, secret ); }
        elif( type=="HS384" ){ return HS384::verify( token, secret ); }
        elif( type=="HS512" ){ return HS512::verify( token, secret ); } 
        
      throw;
    } catch(...) { process::error("invalid token"); } return false; }

    string_t encode ( const string_t& token, string_t secret, string_t type="HS256" ){ try {

          if( type=="XOR"   ){ return XOR  ::encode( token, secret ); }
        elif( type=="HS256" ){ return HS256::encode( token, secret ); }
        elif( type=="HS384" ){ return HS384::encode( token, secret ); }
        elif( type=="HS512" ){ return HS512::encode( token, secret ); }

      throw;  
    } catch(...) { process::error( "jwt algorithm not found" ); } return nullptr; }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif