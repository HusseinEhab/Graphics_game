#version 330 core
// The first line of any shader must a '#version" command which defines the glsl version to use

// This will be the output (you can choose any name) and it will be sent to the frame buffer (back buffer of the window).
out vec4 frag_color;
in vec4 gl_FragCoord ;
vec2 iResolution=vec2(1280,720);
uniform int keyboard;
uniform vec2 mouse;
//void drawCircle (vec2 pos, float diameter, vec4 color1)
//    {
//        pos.x*=iResolution.x/iResolution.y;// normalizing
//        float dist = length(pos);
//
//        if(dist < diameter){
//        frag_color=color1;
//        }
//
//    }
//vec2 calcCenter(float x , float y, vec2 screenPos  )
//    {
//        screenPos.x -= x;
//        screenPos.y -= y;
//        return screenPos;
//    }
//
//void drawSmile(vec2 screenPos)
//    {
//        // Draw Circle 1
//        screenPos=calcCenter(0.5,0.5,screenPos);
//        drawCircle(screenPos,0.2,vec4(1.0,1.0,1.0,1.0));
//
//        // Draw Smile
//        // Draw Circle 2
//        screenPos=calcCenter(0,-0.01,screenPos);
//        drawCircle(screenPos,0.15,vec4(0.0,0.0,0.0,1.0));
//
//
//        // Draw Circle 3
//        screenPos=calcCenter(0,0.015,screenPos);
//        drawCircle(screenPos,0.15,vec4(1.0,1.0,1.0,1.0));
//
//        // Draw Circle 4
//        screenPos=calcCenter(0.05,0.075,screenPos);
//        drawCircle(screenPos,0.03,vec4(0.0,0.0,0.0,1.0));
//
//        // Draw Circle 5
//        screenPos=calcCenter(-0.1,0,screenPos);
//        drawCircle(screenPos,0.03,vec4(0.0,0.0,0.0,1.0));
//
//    }
//    void drawPacman(vec2 screenPos)
//    {
//
//
//        // Draw Circle 1
//        screenPos=calcCenter(0.5,0.5,screenPos);
//        drawCircle(screenPos,0.2,vec4(1.0,1.0,1.0,1.0));
//
//        // Draw Circle 2
//        screenPos=calcCenter(0,0.12,screenPos);
//        drawCircle(screenPos,0.03,vec4(0.0,0.0,0.0,1.0));
//
//        screenPos=calcCenter(0,-0.12,screenPos);
//
//         float m1=1.0;
//         float m2=-1.0;
//            if ((screenPos.x*m1) >screenPos.y && (screenPos.x*m2) <screenPos.y)
//                frag_color =vec4(0.0,0.0,0.0,1.0);
//
//    }


#define PI 3.14159265359
#define TWO_PI 6.28318530718

vec4 circle(vec2 uv, vec2 pos, float rad, vec3 color) {
    float d = length(pos - uv) - rad;
    float t = clamp(d, 0.0, 1.0);
    return vec4(color, 1.0 - t);
}


vec3 rgb(float r, float g, float b) {
    return vec3(r / 255.0, g / 255.0, b / 255.0);
}
void mainImage( out vec4 fragColor, vec2 fragCoord )
{
    // vec2 uv = fragCoord.xy / iResolution.xy;

    vec2 circleUV = fragCoord.xy;
    vec2 circleCenter = iResolution.xy * 0.5;
    float radius = 0.5 * iResolution.y;

    vec2 center = 2.0 * vec2(fragCoord.xy - 0.5 * iResolution.xy) / iResolution.y;

    vec2 uv = (vec2( 2.0 * fragCoord - iResolution.xy ) / iResolution.y);

    int N =3;

    float angle = 1.57;

    mat2 rotate = mat2(cos(angle), -sin(angle), sin(angle), cos(angle));

    //uv = uv * rotate;
    uv  = uv* rotate;

    float a = atan(uv.x,uv.y) + PI,
    r = TWO_PI / float(N),
    d = cos(floor(0.5 + a / r) * r - a) * length(uv);

    vec4 layer1 = vec4(rgb(0.0, 0.0, 0.0),1.0);

    vec3 red = rgb(256.0, 256.0, 256.0);
    vec4 layer2 = circle(circleUV, circleCenter, radius, red);
    layer2 = layer2 - vec4(smoothstep(0.41,0.4,d));

    // Blend the two
    frag_color = mix(layer1, layer2, layer2.a);
}
float squareShape (in vec2 centerShape, in float size, in vec2 point)
{
    float mask = 0.0;

    float u = point[0];
    float v = point[1];


    float minX = centerShape[0] - (size / 2.0);
    float maxX = centerShape[0] + (size / 2.0);
    float minY = centerShape[1] - (size / 2.0);
    float maxY = centerShape[1] + (size / 2.0);

    if ( u > minX && u < maxX && v > minY && v < maxY) mask = 1.0;

    return mask;

}


void main() {
    
    // Here we just output a constant color which is red (R=1, G=0, B=0, A=1)
  
    frag_color=vec4(0.0,0.0,0.0,1.0);

    
//    vec2 screenPos = (gl_FragCoord.xy/iResolution.xy);
//    screenPos.x -= mouse.x/iResolution.x-0.5;
//    screenPos.y += mouse.y/iResolution.y-0.5;
//    if(keyboard==1)
//        drawSmile(screenPos);
//    else if (keyboard==2)
//        drawPacman(screenPos);
        vec2 circleUV = gl_FragCoord.xy + vec2(iResolution.x*0.3,0);
        vec2 circleCenter = iResolution.xy * 0.5;
        float radius = 0.25 * iResolution.y;

        vec2 center = (2.0 * vec2(gl_FragCoord.xy - 0.9 * iResolution.xy) / iResolution.y) - 5;

        vec2 uv = (vec2( 2.0 * gl_FragCoord.xy - iResolution.xy ) / iResolution.y) + vec2(1.07,0) ;

        int N =3;

        float angle = 1.57;

        mat2 rotate = mat2(cos(angle), -sin(angle), sin(angle), cos(angle));

        //uv = uv * rotate;
        uv  = uv* rotate;

        float a = atan(uv.x,uv.y) + PI,
        r = TWO_PI / float(N),
        d = cos(floor(0.5 + a / r) * r - a) * 2* length(uv);

        vec4 layer1 = vec4(rgb(0.0, 0.0, 0.0),1.0);

        vec3 red = rgb(256.0, 256.0, 256.0);

        vec2 circleUV2 = gl_FragCoord.xy - vec2(iResolution.x*0.3,0);
        vec2 circleCenter2 = iResolution.xy * 0.5;
        float radius2 = 0.25 * iResolution.y;
        vec4 layer3 = circle(circleUV2, circleCenter2, radius2, red) ;
        vec2 squareUV = (vec2( 2.0 * gl_FragCoord.xy - iResolution.xy ) / iResolution.y) - vec2(iResolution.x*0.3,0)  ;
        float mask = squareShape(center,0.8, squareUV);
        vec3 col = vec3(mask);
        vec4 col2 = vec4(col,0);



    // convert inShape mask to color




    // Output to screen


        vec4 layer2 = circle(circleUV, circleCenter, radius, red) + layer3;
        layer2 = layer2 - vec4(smoothstep(0.41,0.4,d)) - col2;

        // Blend the two
        frag_color = mix(layer1, layer2, layer2.a);
    }