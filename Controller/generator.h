#include <nodepp/nodepp.h>
#include <nodepp/crypto.h>
#include <nodepp/timer.h>
#include <raylib/game.h>
#include <nodepp/fs.h>

string_t chars = "abcdefghijklmnopqrstuvwxyzab";

void generator() {

    chars = chars.to_upper_case(); rl::Init( 300, 180, 60, "Dchat" );
    auto font = rl::LoadFont("./www/assets/font/font.png");

    ptr_t<rl::Color> colors ({
        rl::GetColor( 0x6636fcff ),
        rl::GetColor( 0x50d35aff ),
        rl::GetColor( 0xffa500ff ),
        rl::GetColor( 0xff0d3bff )
    });

    rl::onClose([=](){ rl::UnloadFont( font ); });

    rl::onDraw([=](){

        rl::ClearBackground( rl::GetColor( 0x222222ff ) ); // 0x0d0d0dff
        chars = chars.sort([=]( char a, char b ){ return rand()%2==0; });

        for( auto x=15; x--; ){
            auto y = rl::Vector2({ rand()%300*1.0f, rand()%180*1.0f });
            auto a = rand() %30 * 1.0f; rl::DrawCircleV( y, a, rl::WHITE );
        }

        uint y=0,z=0; for( auto &x: chars ){ 
            
            auto c = colors[ rand() % colors.size() ];
            auto b = string::to_string(x);

            if( y>240 ){ y=0; z+=40; }

            rl::DrawTextPro( 
              font, b.get(), rl::Vector2({ y+25.0f, z+25.0f }),
                             rl::Vector2({   10.0f,   10.0f }),
                             0, 40, 0, c 
            );

            y+=40;

        }

        for( auto x=15; x--; ){
            auto y = rl::Vector2({ rand()%300*1.0f, rand()%180*1.0f });
            auto z = rl::Vector2({ rand()%300*1.0f, rand()%180*1.0f });
            rl::DrawLineEx( y, z, 1, rl::WHITE );
        }

    });

    process::add([=](){
        static int x=10;
    coStart

        while( x-->0 ){ do {

            int size = 0;
            auto img = rl::LoadImageFromScreen(); 
            auto raw = rl::ExportImageToMemory( img, ".png", &size );

            auto out = fs::writable( regex::format( "./www/captchat/${0}.html", chars ) );
            auto str = string_t( (char*) raw, size );
            console::log( "->", chars, chars.size() );

            out.write("<img src=\"");
            out.write("data:image/png;base64, ");
            out.write( encoder::base64::get(str) );
            out.write("\">");

            rl::UnloadImage( img );

        } while(0); coDelay(1); srand( process::now() ); } rl::Close();

    coStop
    });

}