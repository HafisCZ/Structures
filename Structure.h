#pragma once

#include <memory>
#include <stdexcept>

template <class TYPE> struct basic_node {
	std::unique_ptr<basic_node<TYPE>> m_next = nullptr;
	const TYPE m_val;
	basic_node(const TYPE& val) : m_val(val) {}
};

template <class TYPE> class basic_structure_interface {
	public:
		virtual const TYPE get() = 0;
		virtual const TYPE& peek() const = 0;
		virtual void add(const TYPE& val) = 0;
		virtual inline bool empty() const = 0;
};

template <class TYPE> class Stack : public basic_structure_interface<TYPE> {
	protected:
		std::unique_ptr<basic_node<TYPE>> front = nullptr;
	public:
		virtual const TYPE get() override;
		virtual const TYPE& peek() const override;
		virtual void add(const TYPE& val) override;
		virtual inline bool empty() const override;
};

template <class TYPE> class Queue : public basic_structure_interface<TYPE> {
	protected:
		std::unique_ptr<basic_node<TYPE>> front = nullptr;
		basic_node<TYPE> *end = nullptr;
	public:
		virtual const TYPE get() override;
		virtual const TYPE& peek() const override;
		virtual void add(const TYPE& val) override;
		virtual inline bool empty() const override;
};

template <class TYPE> const TYPE Stack<TYPE>::get() {
	TYPE value = front->m_val;
	front = std::move(front->m_next);
	return value;
}

template <class TYPE> const TYPE& Stack<TYPE>::peek() const {
	return front->m_val;
}

template <class TYPE> void Stack<TYPE>::add(const TYPE& val) {
	auto item = std::make_unique<basic_node<TYPE>>(val);
	if (front != nullptr) item->m_next = std::move(front);
	front = std::move(item);
}

template <class TYPE> inline bool Queue<TYPE>::empty() const {
	return (front == nullptr);
}

template <class TYPE> const TYPE Queue<TYPE>::get() {
	TYPE value = front->m_val;
	front = std::move(front->m_next);
	return value;
}

template <class TYPE> const TYPE& Queue<TYPE>::peek() const {
	return front->m_val;
}

template <class TYPE> void Queue<TYPE>::add(const TYPE& val) {
	auto item = std::make_unique<basic_node<TYPE>>(val);
	(front == nullptr ? front : end->m_next) = std::move(item);
	end = (end == nullptr ? front : end->m_next).get();
}

template <class TYPE> inline bool Stack<TYPE>::empty() const {
	return (front == nullptr);
}