
#include "GrapplingHook.hpp"

GrapplingHook::GrapplingHook() { }

GrapplingHook::GrapplingHook(float xtemp, float ytemp, float xv, float yv, int g) {
	x = xtemp;
	y = ytemp;
	xvel = xv;
	yvel = yv;
	gameobjPlacement = g;
	hitbox = Circle(16 * SIZECHANGE, 16 * SIZECHANGE, 16 * SIZECHANGE);
}

bool GrapplingHook::draw(vector<GameObject*>& gameobjs, Stage* stage) {
	hitbox.x = hitbox.basex + x;
	hitbox.y = hitbox.basey + y;

	return true;
}

/*
Shut up you weeb. You're so mean. You're a nerd for melee and you're very mean. Hat. Bill Cypher. EATED> 
Mabe.bdsfnjdsjdsjnjsjfdk,eksfhjehfhdfedjhfjshfshfjujiwjjkhddbfbahsd.lfjhsdiujkmnjsdjkanrjwdhfsuhawhufhysudhnfsjkahdwaknmadnfehksjdn
j cffmgcgfbmvgn dsffffnnnnnksjfsn nsvcmsbvvvvvvvvnfwlsdikjds,ibn vfsncnvmcfknbkvus,hib
yfhd,knlsmnhbjn,kzms,mncmjk,dxcmbhmcx,xc,vjx,d,vgmc.
DUmmy weed. How do you feel to be called a plant. me wondrr. olleh, beew. ym eman si imis. 
i tsuj detnaw ot yas taht i ezigolopa rof gnieb os naem ot uoy. siht saw lla tsuj a bmud knarp. m'i yrros. 
kcuf nylidam dna reh bmud sdneirf. ho ym hsog taht tog tuo fo lortnoc. seigolopa rof eht dab sdrow. i tsuj etah meht os hcum. 
tub, yldas, uoy era a beew. os uoy t'nac emalb em. eyb-eyb!
*/

void GrapplingHook::customDraw(RenderWindow* window) {
	if (!textureDraw) {
		cout << "Texture loaded!\n";
		setTexture(window->loadTexture("res/gfx/grapple.png"));
		chain = Entity(x, y, window->loadTexture("res/gfx/chain.png"));
		chain.show_width *= SIZECHANGE;
		chain.show_height *= SIZECHANGE;
		fullPicSize();
		show_width *= SIZECHANGE;
		show_height *= SIZECHANGE;
		textureDraw = true;
		setRect();
	}
}