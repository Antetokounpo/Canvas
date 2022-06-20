uniform float factor;

#define LEN 150

float random (vec2 st)
{
    return fract(sin(dot(st.xy, vec2(12.9898,78.233)))*43758.5453123);
}

vec2 get_random_position(vec2 st)
{
    float x = random(st);
    float y = random(vec2(x, x));
    
    return vec2(x, y);
}

vec2 get_random_velocity(vec2 st)
{
    return get_random_position(st);
}

float toroidal_distance(vec2 a, vec2 b)
{
    float dx = abs(a.x - b.x);
    float dy = abs(a.y - b.y);
 
    if (dx > 400.0)
        dx = 800.0 - dx;
 
    if (dy > 225.0)
        dy = 450.0 - dy;
 
    return sqrt(dx*dx + dy*dy);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    float min_distance = 999999999.0;
    vec3 color;
    for(int i = 1; i<=LEN; ++i)
    {
        vec2 pos = get_random_position(vec2(i, i))*800.0;
        vec2 vel = get_random_velocity(pos)*50.0;
       
        vec2 final_pos = vel*iTime + pos;
        final_pos.x = mod(final_pos.x, 800.0);
        final_pos.y = mod(final_pos.y, 450.0);
        
        float d1 = toroidal_distance(final_pos, fragCoord);
        if(d1 < min_distance)
        {
            min_distance = d1;
            color = vec3(0, min_distance/factor, 1.0);
        }
    }
    
    // Time varying pixel color
    vec3 col = 0.045*color/pow(sqrt(min_distance), -1.0);

    // Output to screen
    fragColor = vec4(col,1.0);
}
