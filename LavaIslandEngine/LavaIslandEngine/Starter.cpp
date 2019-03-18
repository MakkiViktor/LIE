#include "Renderer.h"

int main (){
	VK::Renderer renderer;
	while(!renderer.GetWindow ().IsClosed ()){
		renderer.Draw ();
	}
}