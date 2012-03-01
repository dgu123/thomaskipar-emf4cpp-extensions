/*
 * MemoryManager.h
 *
 *  Created on: Feb 21, 2012
 *      Author: thomas
 */

#ifndef MEMORYMANAGER_H_
#define MEMORYMANAGER_H_

#ifdef ECORECPP_USE_GC

#include <ecore_forward.hpp>
#include <set>
#include <cassert>

namespace ecorecpp
{
namespace memory
{

template <class T>
class managed_ptr;

/**
 * The memory allows to automatically manage the life cycle of EObjects.
 */
class MemoryManager {

private:

	template <class T>
	friend class managed_ptr;

	/** set of all entry points to the managed objects */
	::std::set< ::ecore::EObject_ptr > entries;

	/** set containing all managed objects */
	::std::set< ::ecore::EObject_ptr > managedObjects;

	/** shared instance */
	static MemoryManager* instance;

	long long size;

	template <class T>
	void registerEntry(managed_ptr<T>* pPointer) {
		assert(pPointer != 0);
		assert(pPointer->operator ->() != 0);

		entries.insert(pPointer->operator ->());
		manage(pPointer->operator ->());
	}

	template <class T>
	void deregisterEntry(managed_ptr<T>* pPointer) {
		assert(pPointer != 0);
		assert(pPointer->operator ->() != 0);

		entries.erase(pPointer->operator ->());
	}

	bool needsToRunGc();

	void mark(::std::set< ::ecore::EObject_ptr >& pMarkedObjects);

	void markRec(::std::set< ::ecore::EObject_ptr >& pMarkedObjects, ::ecore::EObject_ptr pRoot);

	void sweep(::std::set< ::ecore::EObject_ptr >& pMarkedObjects);

public:

	/**
	 * Returns the shared instance of the memory manager.
	 */
	static MemoryManager* _instance() {
		if (!instance) {
			instance = new MemoryManager();
		}
		return instance;
	}

	/*
	 * Manages the life cycle of the given object. When a object was passed to this method,
	 * the memory manager will take care of releasing the given object when it is not needed
	 * any more.
	 */
	void manage(::ecore::EObject_ptr object);

	void gc();

	int getNumObjects() {
		return managedObjects.size();
	}
};

template<class T>
class managed_ptr {
private:
	T* pointer;

public:
	managed_ptr(T* pPointer) :
			pointer(pPointer) {
		MemoryManager::_instance()->registerEntry(this);
	}

	~managed_ptr() {
		MemoryManager::_instance()->deregisterEntry(this);
	}

	T * operator->() const {
		return pointer;
	}
};

}
}

#endif /*ECORECPP_USE_GC */

#endif /* MEMORYMANAGER_H_ */
