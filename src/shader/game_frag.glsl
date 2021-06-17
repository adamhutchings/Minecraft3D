#version 410 core

out vec4 fragColor;

in vec2 texCoord;
in float biomeShadingFlag;

uniform sampler2D tex;

void main() {
    
    fragColor = texture(tex, texCoord);

    bool applyBiomeShading = false;
    if (biomeShadingFlag == 1.0f) { // Sides of grass blocks
        // Hacky fix - the color change needs to be applied to the sides of the
        // grass blocks as well, so detect those by checking if the color is
        // "green" enough.
        applyBiomeShading = ( (fragColor.y > fragColor.x) );
        // Unfortunately, the provided texture atlas has these already green, so
        // make them gray.
        if (applyBiomeShading) {
            float brightness = fragColor.y;
            fragColor.x = fragColor.z = brightness;
        }
    } else if (biomeShadingFlag == 2.0f) { // Top of grass blocks
        applyBiomeShading = true;
    }

    if (applyBiomeShading) {
        fragColor *= vec4(0.7f, 1.0f, 0.4f, 1.0f);
    }

}
