#include <igris/series/block.h>
#include <igris/series/iterator.h>
#include <igris/series/series.h>

void *igris::series_iterator::pointer()
{
    return block()->get(num);
}

const void *igris::series_iterator::pointer() const
{
    assert(block());
    return block()->get(num);
}

igris::series_iterator &igris::series_iterator::operator++()
{
    if (num == -1)
        return *this;

    num++;
    if (num == block()->fini)
    {
        block_lnk = block_lnk->next;
        num = 0;
    }

    return *this;
}

igris::series_iterator igris::series_iterator::operator++(int)
{
    igris::series_iterator iter = *this;
    ++*this;
    return iter;
}

igris::series_iterator &igris::series_iterator::operator--()
{
    if (num == -1)
        return *this;

    auto *parent = block()->parent;

    num--;
    if (num == block()->strt - 1)
    {
        block_lnk = block_lnk->prev;
        if (&parent->blocks != block_lnk)
            num = 0;
        else
            num = -1;
    }
    return *this;
}

igris::series_iterator igris::series_iterator::operator--(int)
{
    igris::series_iterator iter = *this;
    --*this;
    return iter;
}

igris::series_block *igris::series_iterator::block()
{
    if (num == -1)
        return nullptr;

    return dlist_entry(block_lnk, series_block, lnk);
}

const igris::series_block *igris::series_iterator::block() const
{
    if (num == -1)
        return nullptr;

    return dlist_entry(block_lnk, series_block, lnk);
}

igris::series_iterator::series_iterator(dlist_head *block_lnk, size_t num)
    : block_lnk(block_lnk), num(num)
{
}
