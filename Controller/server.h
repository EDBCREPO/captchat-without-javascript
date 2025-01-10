#include <nodepp/crypto.h>
#include <nodepp/timer.h>
#include <express/http.h>
#include <jwt/jwt.h>

express_tcp_t captchat_processing() {
    auto app = express::http::add();

    app.POST([=]( express_http_t cli ){
        if( cli.headers["Content-Length"].empty() ){ return; }
        
        auto dec = crypto::decrypt::AES_128_ECB( process::env::get("SECRET") );
        auto len = string::to_ulong( cli.headers["Content-Length"] );
        auto raw = query::parse( "?"+url::normalize(cli.read(len)) );

        if(!jwt::verify( raw["token"], process::env::get("SECRET") ) )
          { cli.send("something went wrong"); }
            dec.update( jwt::decode( raw["token"] ) );

        auto data= json::parse( dec.get() ); auto b = true;
        auto pass= data["pass"].as<string_t>().to_lower_case();

        auto i=6; while( i-->0 ){
            if( string::to_string(pass[i]) != raw[string::to_string(i)] )
              { b = false; break; }
        }

        fetch_t args; cli.send();
                args.method = "GET";
                args.url    = b==false ? data["error"].as<string_t>():
                                         data["done"] .as<string_t>();

        http::fetch( args ).then([=]( http_t raw ){
            stream::pipe( raw, cli );
        }).fail([=]( except_t err ){
            cli.write("something went wrong");
        });

    });

    return app;
}

express_tcp_t image_generator() {
    auto app = express::http::add();

    app.ALL([]( express_http_t cli ){
        cli.render( regex::format( R"(
            <div class="uk-flex uk-flex-center uk-flex-middle">
            <div style="border-radius:100%; width:300px; height:300px; background: #222;"
                 class="uk-overflow-hidden" content><° ./View/captchat/${0}.html °>
            </div></div>
        )", cli.params["key"] ));
    });

    return app;
}

express_tcp_t button_generator() {
    auto app = express::http::add();

    app.ALL([]( express_http_t cli ){ string_t data;

        data += R"( <div class="uk-flex uk-h0 uk-flex-between uk-child-width-1-7"> )";

        uint z=0; for( auto &x: cli.params["pass"] ){
             auto y = cli.params["key"].find(x)[0];
             auto a = y % 7; auto b = ( y-a ) / 7;

             data += regex::format( _STRING_(
                <input type="text" name="${0}" maxlength="1" style="height:100px;" required
                    class="uk-input uk-text-center uk-text-uppercase" placeholder="*" >
                <style>
                    [captchat]:has([name="${0}"]:focus) [content]>img { transform: scale(6) translate( -${1}px, -${2}px ); }
                    [captchat]:has([name="${0}"]:focus) [content]     { transform: rotate(${3}deg); }
                </style>
            ), z, a*42, b*45, (rand()%75)-37 ); z++;

        }
        
        data += R"( </div> )"; cli.send( data );

    });

    return app;
}

express_tcp_t token_generator() {
    auto app = express::http::add(); 

    app.ALL([]( express_http_t cli ){

        auto enc = crypto::encrypt::AES_128_ECB( process::env::get("SECRET") );

        enc.update( json::stringify( object_t({
            { "error", "http://localhost:8000/error" },
            { "done" , "http://localhost:8000/done"  },
            { "pass"   , cli.params["pass"] }  
        }) ));

        cli.send( regex::format( R"( 
            <input type="text" name="token" hidden value="${0}" > 
        )", jwt::encode( enc.get(), process::env::get("SECRET") ) ));

    });

    return app;
}

express_tcp_t captchat() {
    auto app = express::http::add();

    app.USE("/img/:key/:pass", image_generator()  );
    app.USE("/tkn/:key/:pass", token_generator()  );
    app.USE("/btn/:key/:pass", button_generator() );

    app.GET([=]( express_http_t cli ){ srand( process::now() );
            
        auto pass= string_t(6,'\0'); int i=6;
        auto list= fs::read_folder("./View/captchat");
        auto key = list[rand()%list.size()].slice(0,-5);

        while( i-->0 ){ pass[i] = key[rand()%key.size()]; }

        cli.render( regex::format( R"(
            <form class="uk-padding uk-background-dark uk-rounded 
                         uk-flex uk-flex-center uk-flex-wrap
                         uk-child-width-1-1 uk-gap uk-inline"
                action="/snd" method="POST" toggle="1" autocomplete="off"
                enctype="application/x-www-form-urlencoded" captchat >

                <label class="uk-position-top-right uk-margin-2xsmall uk-h1 uk-width" >
                    <input type="radio" name="tog" class="uk-toggle-1" hidden >
                    <icon> close </icon>
                </label>

                <° http://localhost:8000/captchat/img/${0}/${1} °>
                <° http://localhost:8000/captchat/btn/${0}/${1} °>
                <° http://localhost:8000/captchat/tkn/${0}/${1} °>

                <button class="uk-button uk-button-primary uk-icon uk-width-1-1"
                        type="submit" > Send <icon>send</icon> 
                </button>

            </form>
        )", key, pass ));

    });

    return app;
}

void server() {

    auto app = express::http::add();

    app.USE( "captchat", captchat() );
    app.USE( "snd", captchat_processing() );

    app.USE( express::http::file("./View") );

    app.listen("localhost",8000,[=](...){
        console::log("-> http://localhost:8000");
    });
    
}