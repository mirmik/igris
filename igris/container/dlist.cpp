#include <igris/container/dlist.h>

void igris::dlist_node::change_parent(dlist_base *newparent)
{
    if (_parent == newparent)
        return;

    if (_parent != nullptr)
        _parent->_size--;

    if (newparent != nullptr)
        newparent->_size++;

    _parent = newparent;
}

void igris::dlist_node::unlink()
{
    if (is_linked())
    {
        next->prev = prev;
        prev->next = next;
        next = prev = this;
    }
    change_parent(nullptr);
}