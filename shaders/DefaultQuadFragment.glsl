float cyl1(vec2 pos, float t);
float cyl2(vec2 pos, float t);
float or(float a, float b);
float and(float a, float b);
vec2 posToUV(vec2 pos);
vec2 UVToPos(vec2 uv);

int width = 500;
int height = 500;

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

float shape(int f_index, vec2 pos, float t)
{
    if(f_index == 1)
    {
        return cyl1(pos, t);
    }
    else if(f_index == 2)
    {
        return cyl2(pos, t);
    }
    else
    {
        return 0.0;
    }
}

vec3 colorLookup(int f_index, vec2 pos)
{
    if(f_index == 1)
    {
        return texture2D(iChannel0, posToUV(pos)).xyz;
    }
    else if(f_index == 2)
    {
        return texture2D(iChannel1, posToUV(pos)).xyz;
    }
    else
    {
        return vec3(0.0);
    }
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

vec3 colGet(int f_index, vec2 pos, float t)
{
    if(shape(f_index, pos, t) >= 0)
    {
        //return vec3(0.0, 1.0, 0.0);
        return colorLookup(f_index, pos);
    }
    else
    {
        vec3 total_col = vec3(0.0);
        float total_weight = 0;
        for(float x = 0; x<1.0; x += 1.0 / 10)
        {
            for(float y = 0; y<1.0; y += 1.0 / 10)
            {

            }
        }
        return vec3(1.0);
    }
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
    vec3 c1 = ;//colGet(1, pos, t);
    vec3 c2 = ;//colGet(2, pos, t);

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
      //s = vec3(1.0);
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
/*
    float f1 = f[0];
    float f2 = f[1];
    float w1 = 0;
    float w2 = 0;
    if(f[1] < 0.0 && f[2] < 0.0)
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
*/
