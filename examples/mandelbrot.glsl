precision highp float;

#define ITERATIONS 300

vec2 pixel_to_cplx(vec2 pixel_pos, float zoom, vec2 center)
{
    vec2 uv = ((pixel_pos/iResolution.xy)*4.0 - 2.0);
    uv.y = -uv.y;
    
    return uv/zoom + center;
}

vec2 complex_pow(vec2 cplx, float n)
{
    float t = atan(cplx.y, cplx.x);
    float r = length(cplx);
    
    r = pow(r, n);
    t = t*n;
    
    return vec2(r*cos(t), r*sin(t));
}

int iterate(vec2 cplx)
{
    vec2 z = vec2(0, 0);
    
    for(int i = 0; i<ITERATIONS; ++i)
    {
        z = complex_pow(z, 2.0) + cplx;
        
        if(length(z) > 2.0)
        {
            return i+1;
        }
    }
    return 0;
}


void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord/iResolution.xy;
    float b = float(iterate(pixel_to_cplx(fragCoord, exp(0), vec2(-0.06783611264225832, 0.6617460391250546))))/float(ITERATIONS);

    // Time varying pixel color
    vec3 col = sqrt(b)*vec3(1.0, 1.0, 1.0);
    // Output to screen
    fragColor = vec4(col,1.0);
}
