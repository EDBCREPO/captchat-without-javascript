#pragma once
#define SECRET "HELLO_WORLD!"

/*────────────────────────────────────────────────────────────────────────────*/

namespace { string_t token_generator( express_http_t cli ) {

    auto tkn = json::stringify( object_t({
        { "auth", cli.params["tkn"] },
        { "expr", process::now()    }
    }) );

    return( regex::format(
        R"(<input type="text" name="token" hidden value="${0}">)",
        jwt::encode( tkn, SECRET )
    ));

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace { string_t button_generator( express_http_t cli ) { string_t data;

    data += R"( <div class="uk-flex uk-h0 uk-flex-between uk-child-width-1-6"> )";

    uint z=0; for( auto &x: cli.params["pass"] ){
         auto y = cli.params["key"].find(x)[0];
         auto a = y %7; auto b = ( y - a ) / 7;
         data += regex::format( _STRING_ ( <div style="padding:2px;">
         <input class="uk-input uk-text-center uk-text-uppercase" pattern="[A-Za-z]"
                name="${0}" maxlength="1" style="height:100px; width:100%;"
                required autocomplete="off" placeholder="*" type="text"
         > <style>
         [captchat]:has([name="${0}"]:focus) [content]       { transform: rotate( ${3}deg ); }
         [captchat]:has([name="${0}"]:focus) [content]>[img] { transform: scale(6) translate( -${1}px, -${2}px ); }
         </style> </div> ), z, a*41, b*46, (rand()%75)-37 ); z++;
    }

    data += R"( </div> )"; return data;

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace controller { void server() {
    auto app = express::http::add();

    /*─······································································─*/

    auto captchat_data = fs::read_file( "./View/assets/css/captchat_name.txt" );
    auto captchat_list = regex::split ( captchat_data, '\n' );
         captchat_list.pop();

    /*─······································································─*/

    app.POST([=]( express_http_t cli ){ try {
        if(!cli.headers.has("Content-Length") ){ throw ""; }
        if( cli.path != "/" ) /*------------*/ { throw ""; }

        auto len = string::to_ulong( cli.headers["Content-Length"] );
        auto raw = query::parse( "?"+url::normalize(cli.read(len)) );

        if( !jwt::verify( raw["token"], SECRET ) )
          { throw ""; }

        auto obj = json::parse( jwt::decode( raw["token"] ) );
        auto tkn = obj[ "auth" ].as<string_t>();
        auto idx = tkn[0]%captchat_list.size();
        auto key = captchat_list[idx];

        if( (process::now()-obj["expr"].as<ulong>()) > TIME_MINUTES(2) )
          { throw ""; } for( auto i=6; i-->0; ) {
        if( raw[string::to_string(i)] /*---------------------*/ .to_upper_case() !=
            string::to_string(key[(tkn[i+1]+tkn[i])%key.size()]).to_upper_case()
        ) { throw ""; } }

        cli.status(200).send_json( object_t({
            { "message", HTTP_NODEPP::_get_http_status(200) },
            { "status" , 200 }
        }) );

    } catch(...) {

        cli.status(404).send_json( object_t({
            { "message", HTTP_NODEPP::_get_http_status(404) },
            { "status" , 404 }
        }) );

    } });

    /*─······································································─*/

    app.GET([=]( express_http_t cli ){ try {
        if( cli.path != "/" ){ return; }

        auto sha = crypto::hash::SHA256();
             sha.update( string::to_string( rand() ) );
             sha.update( string::to_string(process::now()) );

        auto tkn = encoder::base64::atob(sha.get());
        auto idx = tkn[0] % captchat_list.size();
        auto pass= string_t( 6, '\0' );
        auto key = captchat_list[idx];

        for( auto i=6; i-->0; )
           { pass[i]=key[(tkn[i+1]+tkn[i])%key.size()]; }

        cli.params["tkn"] = tkn ; cli.params["key"] = key;
        cli.params["pass"]= pass;

        cli.params["CAPTCHAT"] = regex::format(_STRING_(
            <div class="uk-flex uk-flex-center uk-flex-middle">
            <div style="border-radius:100%; width:300px; height:300px; background: #222;"
                 class="uk-overflow-hidden" content>
            <div   img="${2}"> </div> </div> </div> ${1} ${0}
        ), token_generator(cli), button_generator(cli), idx );

        cli.status(200).render( file_t( "./View/index.html", "r" ) );

    } catch(...) { cli.status(404).send("something went wrong"); } });

    /*─······································································─*/

    app.USE( express::http::file( "./View" ) );

    /*─······································································─*/

    app.listen( "localhost", 8000, [=]( socket_t ){
        console::log( "-> http://localhost:8000" );
    });

}}

#undef SECRET

/*────────────────────────────────────────────────────────────────────────────*/
