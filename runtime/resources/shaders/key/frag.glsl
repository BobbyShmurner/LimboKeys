#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D keyTexture;
uniform sampler2D overlayTexture;
uniform vec4 col;
uniform vec4 overlayCol;

void main() {
	vec4 base = texture(keyTexture, TexCoord) * col;
	vec4 overlay = texture(overlayTexture, TexCoord) * overlayCol;
	FragColor = vec4(mix(base.xyz, overlay.xyz, overlay.a), base.a);
}