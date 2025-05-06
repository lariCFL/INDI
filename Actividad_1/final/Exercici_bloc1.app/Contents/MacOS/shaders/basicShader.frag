#version 330 core

in vec4 vertex_color;
in vec4 vertex_pos;
in float vIsTerra;

out vec4 FragColor;

void main() {
    if (vIsTerra >= 0.5) {
        const float stripeWidth = 10.0;
        int pixelX = int(mod(gl_FragCoord.x / stripeWidth, 2));
        int pixelY = int(mod(gl_FragCoord.y / stripeWidth, 2));

        bool Strip_Horiz = pixelY == 0;
        bool Strip_Vert = pixelX == 0;

        bool interc = Strip_Horiz && Strip_Vert;
        bool Pref_Horiz = mod(int(gl_FragCoord.x / stripeWidth) + int(gl_FragCoord.y / stripeWidth), 4) < 2;
    
        if ((Strip_Horiz && !Strip_Vert) || (interc && Pref_Horiz)) {
            FragColor = vertex_color;
        } 
        else if (Strip_Vert && !Strip_Horiz || (interc && !Pref_Horiz)) {
            FragColor = vec4(mix(vertex_color.rgb, vec3(0.0), 0.5), 1.0);
        } 
        else {
            FragColor = vec4(1,1,1, 0.0);
        }
    }
    else { 
        FragColor = vertex_color;
    }
}