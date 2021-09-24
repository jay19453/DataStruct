#pragma once
#include <memory>

namespace wnet {
	template <typename T>
	class ListSlot {
	public:
		ListSlot() {};
		~ListSlot() {};
	public:
		void setNext(std::shared_ptr<T> ptr) { _next = ptr; }
		std::shared_ptr<T> getNext() { return _next; }
		void setPrev(std::shared_ptr<T> ptr) { _prev = ptr; }
		std::shared_ptr<T> getPrev() { _prev.lock(); }
	private:
		std::shared_ptr<T> _next;
		std::weak_ptr<T> _prev;
	};
}