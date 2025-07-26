#version 100
precision mediump float;

varying vec2 fragTexCoord;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

const float renderWidth = 640.0;
const float renderHeight = 480.0;
const float offset = 0.0;

uniform float time;

void main() {
    float frequency = renderHeight / 10.0;

    float globalPos = ((fragTexCoord.y + offset) * frequency) - time;
    float wavePos = cos((fract(globalPos) - 0.5) * 3.14159);

    vec4 texelColour = texture2D(texture0, fragTexCoord);
    vec3 overlayColour = vec3(1.0, 1.0, 1.0);
    vec3 mixedColour = mix(vec3(texelColour * 0.75), vec3(texelColour), wavePos);
    vec3 multipliedColour = mix(mixedColour, overlayColour, 0.1);

    gl_FragColor = vec4(multipliedColour, 1.0);
}
