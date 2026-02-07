#include <nodepp/nodepp.h>
#include <nodepp/crypto.h>
#include <nodepp/timer.h>
#include <nodepp/fs.h>

#include "game.h"

string_t chars = "abcdefghijklmnopqrstuvwxyzab";

namespace controller { void generator() {

    chars = chars.to_upper_case(); rl::Init( 300, 180, 60, "captchat" );
    auto file = fs::writable("./View/assets/css/captchat_data.css");
    auto name = fs::writable("./View/assets/css/captchat_name.txt");
    auto font = rl::LoadFont("./View/assets/font/font.png");

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
        static int x=0;
    coStart

        for( x=0; x<=30; x++ ){ do {

            int size = 0;
            auto nme = chars.copy();
            auto img = rl::LoadImageFromScreen(); 
            auto raw = rl::ExportImageToMemory( img, ".png", &size );
            auto str = string_t( (char*) raw, size ); console::log( x,"->",nme );

            file.write( regex::format( "[img=\"${0}\"]{" ,x ) );

            file.write( "background-repeat: no-repeat;" );
            file.write( "width: 300px; height: 180px;"  );
            file.write( "image-rendering: pixelated;"   );
            file.write( "transform-origin: 0px 0px;"    );
            file.write( "transform: scale(6); "         );

            file.write( "background-image:url('"  );
            file.write( "data:image/png;base64, " ); 
            file.write( encoder::base64::get(str) );
            name.write( nme + ( x<30 ? "\n": "" ) );
            file.write( regex::format("'); }") );

            rl::UnloadImage( img );

        } while(0); coDelay(1); srand( process::now() ); } rl::Close();

    coStop
    });

}}