#version 330 core

uniform vec2 iResolution;
uniform float iTime;

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    fragColor = vec4(0, 1, 0,1.0);
}

void main()
{
    mainImage(gl_FragColor, gl_FragCoord.xy);
}