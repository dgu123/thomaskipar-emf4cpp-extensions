/*
 * MemoryManager.cpp
 *
 *  Created on: Feb 21, 2012
 *      Author: thomas
 */

#include <cassert>
#include "MemoryManager.hpp"
#include <ecore/EClass.hpp>
#include <ecorecpp/mapping.hpp>

#include <iostream>

#ifdef ECORECPP_USE_GC

using namespace ::ecorecpp::memory;

MemoryManager* MemoryManager::instance = 0;

bool MemoryManager::needsToRunGc() {
	return true;
}

void MemoryManager::manage(::ecore::EObject_ptr object) {
	assert(object != 0);
	if (needsToRunGc()) {
		gc();
	}
	managedObjects.insert(object);
	::std::cout << "manage object: " << sizeof(*object) << ::std::endl;
	size += sizeof(*object);

}

void MemoryManager::gc() {
	::std::set< ::ecore::EObject_ptr > marked;
	mark(marked);
	sweep(marked);
}

void MemoryManager::mark(::std::set< ::ecore::EObject_ptr >& pMarkedObjects) {
	::std::set< ::ecore::EObject_ptr >::iterator it;
	for (it = entries.begin(); it != entries.end(); ++it) {
		markRec(pMarkedObjects, *it);
	}
}

void MemoryManager::markRec(::std::set< ::ecore::EObject_ptr >& pMarkedObjects,
		::ecore::EObject_ptr pRoot) {
	if (!pMarkedObjects.count(pRoot)) {
		pMarkedObjects.insert(pRoot);

		// process children
		::ecore::EClass_ptr cl = pRoot->eClass();
		::ecorecpp::mapping::EList< ecore::EReference > const& references =
				cl->getEAllReferences();

		for (size_t i = 0; i < references.size(); i++) {
			::ecore::EReference_ptr current_ref = references[i];
			if (pRoot->eIsSet(current_ref)) {
				ecorecpp::mapping::any any = pRoot->eGet(current_ref);
				if (current_ref->getUpperBound() != 1) {
					::ecorecpp::mapping::EList_ptr children =
							ecorecpp::mapping::any::any_cast<
									ecorecpp::mapping::EList_ptr >(any);
					for (size_t j = 0; j < children->size(); j++) {
						markRec(pMarkedObjects, (*children)[j]);
					}
				} else {
					::ecore::EObject_ptr child =
							ecorecpp::mapping::any::any_cast<
									::ecore::EObject_ptr >(any);
					if (child) {
						markRec(pMarkedObjects, child);
					}

				}
			}
		}
	}
}

void MemoryManager::sweep(::std::set< ::ecore::EObject_ptr >& pMarkedObjects) {
	::std::set< ::ecore::EObject_ptr > unmarked;
	::std::set< ::ecore::EObject_ptr >::iterator it;
	for (it = managedObjects.begin(); it != managedObjects.end(); ++it) {
		if (!pMarkedObjects.count(*it)) {
			unmarked.insert(*it);
		}
	}
	for (it = unmarked.begin(); it != unmarked.end(); ++it) {
		delete *it;
		managedObjects.erase(*it);
	}
}

#endif /* ECORECPP_USE_GC */
