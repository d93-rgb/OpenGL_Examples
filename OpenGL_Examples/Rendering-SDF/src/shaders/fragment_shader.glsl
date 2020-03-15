#version 330 core
// Signed distance function demo of a circle.
// Main code stolen from iq: https://www.shadertoy.com/view/3ltSW2
// Nevertheless, a bit of modification was needed.

uniform vec2 resolution;

float sdCircle( in vec2 p, in float r ) 
{
    return length(p)-r;
}

void main()
{
	vec2 p = (2.0 * gl_FragCoord.xy - resolution)/resolution.y;

	float d = sdCircle(p,0.5);
    
	// coloring
    vec3 col = vec3(1.0) - sign(d)*vec3(0.1,0.4,0.7);
    col *= 1.0 - exp(-3.0*abs(d));
	col *= 0.8 + 0.2*cos(150.0*d);
	col = mix( col, vec3(1.0), 1.0-smoothstep(0.0,0.01,abs(d)) );

	gl_FragColor = vec4(col,1.0);
}