float uni(float a, float b)
{
    float max = a;
    if(a < b)
    {
        max = b;
    }
    return max;
}

float intersect(float a, float b)
{
    float min = a;
    if(a > b)
    {
        min = b;
    }
    return min;
}

bool ifpos(float n)
{
    if(n >= 0.0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

vec3 my_model(vec2 pos, float t)
{
    // pos : position (x, y, t)
    // a : ?
    // S : color

    vec3 s = vec3(0.0);

    // ?
    float a1 = 1.0; //5
    float a2 = 1.0; //2
    float a3 = 1.0; //2
    float a4 = 1.0; //0.75

    //a1=a[1];
    //a2=a[2];
    //a3=a[3];
    //a4=a[4];

    // time warp
    //zwarp = -10+pos[2]*20;
    float zwarp = t;

    // disk centered at (1.0, 0)
    float disk1 = 1.0 - pow((pos[0] - 1.0), 2.0) - pos[1] * pos[1];
    float disk2 = 1.0 - pow((pos[0] + 1.5), 2.0) - pos[1] * pos[1];
    float ddisk = uni(disk1, disk2);

    // cross
    float bl1 = intersect(intersect(intersect((pos[0] + 1.0), (0.0 - pos[0])), (pos[1] - 2.0)), (5.0 - pos[1]));
    float bl2 = intersect(intersect(intersect((pos[1] - 3.0), (4.0 - pos[1])) , (pos[0] + 2.0)), (1.0 - pos[0]));
    float cross = uni(bl1, bl2);

    // space-time blending

    // 1 section exists for pos3 <= 0
    float cyl1 = intersect(ddisk, (-zwarp));

    // 2 section exists fo pos3 >= 1
    float cyl2 = intersect(cross, (zwarp-1.0));

    float f1 = cyl1;
    float f2 = cyl2;
    float f3 = intersect((t + 10.0), (10.0 - t));
    
    float r1 = pow((f1 / a2), 2.0) + pow((f2 / a3), 2.0);
    float r2 = 0.0;

    if (f3 > 0.0)
    {
        r2 = pow((f3 / a4), 2.0);
    }

    float rr = 0.0;
    if(r1 > 0.0)
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
    if(ifpos(bb2diskcross))
    {
        s[0] = 1.0;
        s[1] = 1.0;
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

    // colors of initial objects
    vec4 c1 = texture2D(iChannel0, (pos+vec2(5.0,3.0)) / 10.0);
	vec4 c2 = texture2D(iChannel1, (pos+vec2(5.0,3.0)) / 10.0);
    
    r1 = c1.r;
    float g1 = c1.g;
    float b1 = c1.b;
    r2 = c2.r;
    float g2 = c2.g;
    float b2 = c2.b;

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
        if(f1 > 0.0)
        {
           w1 = 1.0;
           w2 = 0.0;
        }

        if(f2 > 0.0)
        {
           w1 = 0.0;
           w2 = 1.0;
        }
      }

      s[0] = r1 * w1 + r2 * w2;
      s[1] = g1 * w1 + g2 * w2;
      s[2] = b1 * w1 + b2 * w2;

    }
    return s;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
  //vec2 uv = fragCoord.xy / iResolution.xy;
  //fragColor = vec4(uv,0.5+0.5*sin(iGlobalTime),1.0);


  vec2 uv = fragCoord.xy / iResolution.xy;

  float t = (float(iGlobalTime) * 2.0 - 10.0);

  vec2 pos = vec2( uv.x * 10.0 - 5.0, uv.y * 10.0 - 3.0);
  float ztime = t;

  vec3 color = my_model(pos, ztime);

  //vec3 color = vec3(0.0);
  //color = bw(pos, 1.0);

  fragColor = vec4(color, 1.0);
}

