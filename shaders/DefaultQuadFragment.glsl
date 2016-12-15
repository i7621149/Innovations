float cyl1(vec2 pos, float t);
float cyl2(vec2 pos, float t);
float or(float a, float b);
float and(float a, float b);
vec2 posToUV(vec2 pos);
vec2 UVToPos(vec2 uv);

float cyl1(vec2 pos, float t)
{
    // time warp
    //float zwarp = -10.0 + t * 20.0;
    float zwarp = t;

    // disk centered at (1.0, 0)
    float disk1 = 1.0 - pow((pos[0] - 1.0), 2.0) - pos[1] * pos[1];
    float disk2 = 1.0 - pow((pos[0] + 1.5), 2.0) - pos[1] * pos[1];
    float ddisk = or(disk1, disk2);

    // 1 section exists for zwarp <= 0
    return and(ddisk, (-zwarp));
}

float cyl2(vec2 pos, float t)
{
    // time warp
    //float zwarp = -10.0 + t * 20.0;
    float zwarp = t;

    // cross
    float bl1 = and(and(and((pos[0] + 1.0), (0.0 - pos[0])), (pos[1] - 2.0)), (5.0 - pos[1]));
    float bl2 = and(and(and((pos[1] - 3.0), (4.0 - pos[1])) , (pos[0] + 2.0)), (1.0 - pos[0]));
    float cross = or(bl1, bl2);

    // 2 section exists for zwarp >= 1
    return and(cross, (zwarp-1.0));
}

float or(float a, float b)
{
    /*
    float max = a;
    if(a < b)
    {
        max = b;
    }
    return max;
    */
    return a + b + sqrt(a *a + b * b);
}

float and(float a, float b)
{
    /*
    float min = a;
    if(a > b)
    {
        min = b;
    }
    return min;
    */
    return a + b - sqrt(a * a + b * b);
}

vec2 posToUV(vec2 pos)
{
    return (pos+vec2(5.0,3.0)) / 10.0;
}

vec2 UVToPos(vec2 uv)
{
    return vec2(uv.x * 10.0 - 5.0, uv.y * 10.0 - 3.0);
}

vec3 colBlend(float[2] f, vec3[2] c, int n)
{
    float f1 = f[0];
    float f2 = f[1];
    float w1 = 0;
    float w2 = 0;
    if(f1 < 0.0 && f2 < 0.0)
    {
      float fa1 = abs(f1);
      float fa2 = abs(f2);
      float fsum = fa1 + fa2;
      w1 = fa2 / fsum;
      w2 = fa1 / fsum;
    }
    else
    {
      if(f1 >= 0.0)
      {
         w1 = 1.0;
         w2 = 0.0;
      }

      if(f2 >= 0.0)
      {
         w1 = 0.0;
         w2 = 1.0;
      }
    }

    vec3 s = c[0] * w1 + c[1] * w2;

    return s;
}

vec3 col1(float f0, vec2 pos)
{
    float f1 = and(f0, pos.y);

    float f2 = and(f0,-pos.y);

    vec3 s;
    vec3 c1 = vec3(0.0, 1.0, 0.0);
    vec3 c2 = vec3(0.0, 0.0, 1.0);

    float w1;
    float w2;
    // color distribution in 4D space-time using partition of unity
    if(f1 < 0.0 && f2 < 0.0)
    {
      float fa1 = abs(f1);
      float fa2 = abs(f2);
      float fsum = fa1 + fa2;
      w1 = fa2 / fsum;
      w2 = fa1 / fsum;
    }
    else
    {
      if(f1 >= 0.0)
      {
         w1 = 1.0;
         w2 = 0.0;
      }

      if(f2 >= 0.0)
      {
         w1 = 0.0;
         w2 = 1.0;
      }
    }

    s = c1 * w1 + c2 * w2;

    return s;
}

vec3 col2(float f0, vec2 pos)
{
    float f1 = and(f0, pos.x+0.5);

    float f2 = and(f0, -0.5-pos.x);
    vec3 c1 = vec3(1.0, 1.0, 0.0);
    vec3 c2 = vec3(1.0, 0.0, 1.0);

    float f[2];
    vec3 c[2];
    f[0] = f1;
    f[1] = f2;
    c[0] = c1;
    c[1] = c2;

    return colBlend(f, c, 2);
}

vec3 my_model(vec2 pos, float t)
{
    // pos : position (x, y, t)
    // a : ?
    // S : color

    vec3 s = vec3(0.0);

    // ?
    float a1 = 5.0; //5
    float a2 = 2.0; //2
    float a3 = 2.0; //2
    float a4 = 0.75; //0.75

    //a1=a[1];
    //a2=a[2];
    //a3=a[3];
    //a4=a[4];

    float f1 = cyl1(pos, t);
    float f2 = cyl2(pos, t);
    float f3 = and((t + 10.0), (10.0 - t));

    float r1 = pow((f1 / a2), 2.0) + pow((f2 / a3), 2.0);
    float r2 = 0.0;

    if (f3 >= 0.0)
    {
        r2 = pow((f3 / a4), 2.0);
    }

    float rr = 0.0;
    if(r1 >= 0.0)
    {
        rr = r1 / (r1 + r2);
    }

    float d = 0.0;
    if (rr < 1.0)
    {
        d = a1 * pow((1.0 - rr), 3.0) / (1.0 + rr);
    }

    float bb2diskcross = f1 + f2 + sqrt(pow(f1 , 2.0) + pow(f2, 2.0)) + d;

    /*
    if(bb2diskcross >= 0)
    {
        s[0] = 1.0;
        s[1] = 0.0;
        s[2] = 1.0;
        return s;
    }
    else
    {
        s[0] = 0.0;
        s[1] = 0.0;
        s[2] = 0.0;
        return s;
    }
    */

    s[0] = 0.0;
    s[1] = 0.0;
    s[2] = 0.0;

    vec2 uv = posToUV(pos);
    //vec3 c1 = vec3(texture2D(iChannel0, uv).xyz); // vec3(1.0, 1.0, 0.0);
    //vec3 c2 = vec3(texture2D(iChannel1, uv).xyz); // vec3(1.0, 0.0, 1.0);
    vec3 c1 = col1(f1, pos);
    vec3 c2 = col2(f2, pos);

    r1=1.0;
    float g1=1.0;
    float b1=0.0;
    r2=1.0;
    float g2=0.0;
    float b2=1.0;

    if(bb2diskcross >= 0.0)
    {
      float w1;
      float w2;
      // color distribution in 4D space-time using partition of unity
      if(f1 < 0.0 && f2 < 0.0)
      {
        float fa1 = abs(f1);
        float fa2 = abs(f2);
        float fsum = fa1 + fa2;
        w1 = fa2 / fsum;
        w2 = fa1 / fsum;
      }
      else
      {
        if(f1 >= 0.0)
        {
           w1 = 1.0;
           w2 = 0.0;
        }

        if(f2 >= 0.0)
        {
           w1 = 0.0;
           w2 = 1.0;
        }
      }

      //s[0] = r1 * w1 + r2 * w2;
      //s[1] = g1 * w1 + g2 * w2;
      //s[2] = b1 * w1 + b2 * w2;
      s = c1 * w1 + c2 * w2;
    }
    return s;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
  //vec2 uv = fragCoord.xy / iResolution.xy;
  //fragColor = vec4(uv,0.5+0.5*sin(iGlobalTime),1.0);

  vec2 uv = fragCoord.xy / iResolution.xy;

  float t = (float(iGlobalTime) * 2.0 - 10.0);

  vec2 pos = UVToPos(uv);
  float ztime = t;

  vec3 color = my_model(pos, ztime);

  //vec3 color = vec3(0.0);
  //color = bw(pos, 1.0);

  fragColor = vec4(color, 1.0);
}

// pos toUV:
// (x+5.0) / 10.0
// (y+3.0) / 10.0
