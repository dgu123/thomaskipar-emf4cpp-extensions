/*
 * MemoryManager.h
 *
 *  Created on: Feb 21, 2012
 *      Author: thomas
 */

//#ifdef ECORECPP_USE_GC

#ifndef MEMORYMANAGER_H_
#define MEMORYMANAGER_H_

#include <ecore_forward.hpp>
#include <set>
#include <map>
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

public:

	MemoryManager();

	/** map of all entry points to the managed objects and their count */
	::std::map< ::ecore::EObject_ptr, int> entries;

	/** map containing all managed objects and their size */
	::std::map< ::ecore::EObject_ptr, size_t > managedObjects;

	/** shared instance */
	static MemoryManager* s_instance;

	size_t totalSize;

	size_t maxSize;

	void registerEntry(::ecore::EObject_ptr pPointer) {
		assert(pPointer != 0);
		int count = 1;
		if (entries.count(pPointer) > 0) {
			count = entries[pPointer] + 1;
		}
		entries[pPointer] = count;
	}

	void deregisterEntry(::ecore::EObject_ptr pPointer) {
		assert(pPointer != 0);
		int count = entries[pPointer];
		if (count <= 1) {
			entries.erase(pPointer);
		} else {
			entries[pPointer] = count - 1;
		}
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
		if (!s_instance) {
			s_instance = new MemoryManager();
		}
		return s_instance;
	}

	/*
	 * Manages the life cycle of the given object. When a object was passed to this method,
	 * the memory manager will take care of releasing the given object when it is not needed
	 * any more.
	 */
	void manage(::ecore::EObject_ptr object, size_t size);

	void gc();

	int getNumObjects() {
		return managedObjects.size();
	}

	size_t getSize() {
		return totalSize;
	}
};

template<class T>
class managed_ptr {
private:
	T pointer;

public:
	managed_ptr(T pPointer) :
			pointer(pPointer) {
		MemoryManager::_instance()->registerEntry(get());
	}

	managed_ptr(const managed_ptr<T>& other) :
			pointer(other.get()) {
		MemoryManager::_instance()->registerEntry(get());
	}

	~managed_ptr() {
		MemoryManager::_instance()->deregisterEntry(get());
	}

	T operator->() const {
		return pointer;
	}

	T get() const {
		return pointer;
	}
};

}
}

#endif /* MEMORYMANAGER_H_ */

//#endif /*ECORECPP_USE_GC */
