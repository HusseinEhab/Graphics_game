#version 330 core
// The first line of any shader must a '#version" command which defines the glsl version to use

// This will be the output (you can choose any name) and it will be sent to the frame buffer (back buffer of the window).
out vec4 frag_color;
in vec4 gl_FragCoord ;
vec2 iResolution=vec2(1280,720);
uniform int keyboard;
uniform vec2 mouse;
void drawCircle (vec2 pos, float diameter, vec4 color1)
    {   
        pos.x*=iResolution.x/iResolution.y;// normalizing
        float dist = length(pos);
    
        if(dist < diameter){
        frag_color=color1;
        }
        
    }
vec2 calcCenter(float x , float y, vec2 screenPos  )
    {
        screenPos.x -= x;
        screenPos.y -= y;
        return screenPos;
    }

void drawSmile(vec2 screenPos)
    {
        // Draw Circle 1
        screenPos=calcCenter(0.5,0.5,screenPos);
        drawCircle(screenPos,0.2,vec4(1.0,1.0,1.0,1.0));
        
        // Draw Smile
        // Draw Circle 2
        screenPos=calcCenter(0,-0.01,screenPos);
        drawCircle(screenPos,0.15,vec4(0.0,0.0,0.0,1.0));


        // Draw Circle 3
        screenPos=calcCenter(0,0.015,screenPos);
        drawCircle(screenPos,0.15,vec4(1.0,1.0,1.0,1.0));

        // Draw Circle 4
        screenPos=calcCenter(0.05,0.075,screenPos);
        drawCircle(screenPos,0.03,vec4(0.0,0.0,0.0,1.0));

        // Draw Circle 5
        screenPos=calcCenter(-0.1,0,screenPos);
        drawCircle(screenPos,0.03,vec4(0.0,0.0,0.0,1.0));
    
    }
    void drawPacman(vec2 screenPos)
    {
        

        // Draw Circle 1
        screenPos=calcCenter(0.5,0.5,screenPos);
        drawCircle(screenPos,0.2,vec4(1.0,1.0,1.0,1.0));

        // Draw Circle 2
        screenPos=calcCenter(0,0.12,screenPos);
        drawCircle(screenPos,0.03,vec4(0.0,0.0,0.0,1.0));

        screenPos=calcCenter(0,-0.12,screenPos);
        
         float m1=1.0;
         float m2=-1.0;
            if ((screenPos.x*m1) >screenPos.y && (screenPos.x*m2) <screenPos.y)
                frag_color =vec4(0.0,0.0,0.0,1.0);
        
    }
    void main() {
    
    // Here we just output a constant color which is red (R=1, G=0, B=0, A=1)
  
    frag_color=vec4(0.0,0.0,0.0,1.0);

    
    vec2 screenPos = (gl_FragCoord.xy/iResolution.xy);
    screenPos.x -= mouse.x/iResolution.x-0.5;
    screenPos.y += mouse.y/iResolution.y-0.5;
    if(keyboard==1)
        drawSmile(screenPos);
    else if (keyboard==2)
        drawPacman(screenPos);
    }