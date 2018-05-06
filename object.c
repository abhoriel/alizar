#include "utlist.h"
#include "object.h"

Object *head = NULL;

void objectAdd(Object *obj) {
	DL_PREPEND2(head, obj, prev, next);
}

void objectDelete(Object *obj) {
	DL_DELETE2(head, obj, prev, next);
}

void objectTick() {
	// nb to ben, foreach_safe should not crash if the element is deleted during the iteration
	Object *obj, *temp;
	DL_FOREACH_SAFE2(head, obj, temp, next) {
		if (obj->tick != NULL) {
			obj->tick();
		}
	}
}

void objectSetLocation(Object *obj, int x, int y) {
	obj->x = x;
	obj->y = y;
	// This could also set the object position on the map
}



