#include "NodePool/PoolManager.h"
#include "common.h"
#include <thread>
#include <gtest/gtest.h>
using namespace testing;
using NodePool::PoolManager;
using NodePool::TraceNode;

TEST(poolManger, get_and_give_back)
{
    NodePool::PoolManager pool;
    // new
    TraceNode &_node = pool.GetNode();
    void *p = &_node;
    NodeID id = _node.getId();
    // give back
    pool.freeNode(id);
    TraceNode &_node_01 = pool.GetNode();

    // ref current
    TraceNode &ref_new_node = pool.GetNode(_node_01.getId());

    EXPECT_EQ(p, &_node);
    EXPECT_EQ(ref_new_node, _node);
    // reuse the same id
    EXPECT_EQ(id, _node.getId());
    EXPECT_THROW(pool.GetNode(NodeID(100)), std::out_of_range);
    EXPECT_THROW(pool.GetNode(NodeID(10000)), std::out_of_range);
}

static NodePool::PoolManager g_pool;

void test_node_pool(bool &result)
{
    NodeID it = E_INVALID_NODE;
    for (int i = 0; i < 1000; i++)
    {
        TraceNode &_node = g_pool.GetNode();
        usleep(1000);
        if (_node.getId() == it)
        {
            result = false;
            return;
        }
        g_pool.freeNode(_node);
        // g_pool.freeNode(_node.getId());
    }
    result = true;
}

TEST(poolManger, get_and_give_back_tls)
{
    bool rth1, rth2;
    std::thread t1(test_node_pool, std::ref(rth1));
    std::thread t2(test_node_pool, std::ref(rth2));

    t1.join();
    t2.join();
    EXPECT_TRUE(rth1);
    EXPECT_TRUE(rth2);
}
