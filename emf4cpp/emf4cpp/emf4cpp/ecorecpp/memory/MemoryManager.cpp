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

//#ifdef ECORECPP_USE_GC

using namespace ::ecorecpp::memory;

MemoryManager* MemoryManager::s_instance = 0;

MemoryManager::MemoryManager() {
	maxSize = 16 * 1024 * 1024;
}

bool MemoryManager::needsToRunGc() {
	return totalSize > maxSize;
}

void MemoryManager::manage(::ecore::EObject_ptr object, size_t size) {
	//::std::cout << "manage..." << ::std::endl;
	assert(object != 0);
	if (needsToRunGc()) {
		gc();
	}
	managedObjects.insert(::std::pair< ::ecore::EObject_ptr, size_t >(object, size));
	totalSize += size;
	//::std::cout << "did manage!" << ::std::endl;
}

void MemoryManager::gc() {
	//::std::cout << "run gc..." << ::std::endl;

	::std::set< ::ecore::EObject_ptr > marked;
	mark(marked);
	sweep(marked);
	if (maxSize < totalSize * 1.5) {
		maxSize = totalSize * 1.5;
	}

	//::std::cout << "done run gc!" << ::std::endl;
}

void MemoryManager::mark(::std::set< ::ecore::EObject_ptr >& pMarkedObjects) {
	::std::map< ::ecore::EObject_ptr, int >::iterator it;
	for (it = entries.begin(); it != entries.end(); ++it) {
		markRec(pMarkedObjects, it->first);
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
	::std::map< ::ecore::EObject_ptr, size_t >::iterator it;
	for (it = managedObjects.begin(); it != managedObjects.end(); ++it) {
		if (!pMarkedObjects.count(it->first)) {
			unmarked.insert(it->first);
			totalSize -= it->second;
		}
	}

	::std::set< ::ecore::EObject_ptr >::iterator it2;
	for (it2 = unmarked.begin(); it2 != unmarked.end(); ++it2) {
		delete *it2;
		managedObjects.erase(*it2);
	}
}

//#endif /* ECORECPP_USE_GC */
