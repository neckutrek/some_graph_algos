
#include <vector>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <queue>
#include <stack>
#include <iostream>
#include <deque>
#include <memory>
#include <type_traits>

using namespace std;

namespace my
{
   template <typename T>
   class priority_queue // because STL sucks so bad!
   {
      vector<T> m_min_heap;
   public:
      constexpr bool empty() const noexcept
      {
         return m_min_heap.empty();
      }

      void push_front(const T& t)
      {
         m_min_heap.push_back(t);
         push_heap(m_min_heap.begin(), m_min_heap.end(), [](const T& a, const T& b){ return a < b; });
      }

      void pop_front()
      {
         pop_heap(m_min_heap.begin(), m_min_heap.end());
         m_min_heap.pop_back();
      }

      T& front()
      {
         return m_min_heap.at(0);
      }

      const T& front() const
      {
         return m_min_heap.at(0);
      }

      decltype(m_min_heap.cbegin()) cbegin() const noexcept
      {
         return m_min_heap.cbegin();
      }

      decltype(m_min_heap.cend()) cend() const noexcept
      {
         return m_min_heap.cend();
      }
   };

} // empty namespace

class Graph
{
   unordered_map<size_t, vector<size_t>>
                  m_adj_list; // adjacency list graph representation

   void addEdge_impl(size_t from, size_t to)
   {
      if (m_adj_list[from].empty() ||
          find(m_adj_list[from].cbegin(), m_adj_list[from].cend(), to) == m_adj_list[from].end())
      {
         m_adj_list[from].push_back(to);
      }
   }

   struct breadth_first_search_tag;
   struct depth_first_search_tag;
   struct prims_mst_tag;

   template <typename T, typename U>
   struct is_same_x : is_same<T, U> {
      using type = void;
   };

   template <typename T>
   struct is_same_x<T, breadth_first_search_tag>
   : is_same<T, breadth_first_search_tag> {
      using type = deque<size_t>;
   };

   template <typename T>
   struct is_same_x<T, depth_first_search_tag>
   : is_same<T, depth_first_search_tag> {
      using type = deque<size_t>;
   };

   template <typename T>
   struct is_same_x<T, prims_mst_tag>
   : is_same<T, prims_mst_tag> {
      using type = my::priority_queue<size_t>;
   };

   template <typename tag = breadth_first_search_tag>
   void traverse_impl(size_t start)
   {
      if (m_adj_list.empty())
      {
         return;
      }

      set<size_t> visited;

      using TraverseOrderType =
         typename disjunction<
            is_same_x<tag, breadth_first_search_tag>,
            is_same_x<tag, depth_first_search_tag>,
            is_same_x<tag, prims_mst_tag>
            >::type;

      TraverseOrderType to_visit;

      to_visit.push_front(start);

      while ( !to_visit.empty() )
      {
         size_t curr = to_visit.front();
         to_visit.pop_front();

         cout << curr << ", ";

         if (visited.find(curr) != visited.cend())
         {
            continue;
         }
         visited.insert(curr);

         if (m_adj_list.find(curr) == m_adj_list.cend())
         {
            continue;
         }

         for (const auto& to_node : m_adj_list.at(curr))
         {
            if (visited.find(to_node) != visited.cend() ||
                find(to_visit.cbegin(), to_visit.cend(), to_node) != to_visit.cend())
            {
               continue;
            }

            if constexpr ( is_same<tag, breadth_first_search_tag>::value )
            {
               to_visit.push_back(to_node);
            }
            else if constexpr ( is_same<tag, depth_first_search_tag>::value )
            {
               to_visit.push_front(to_node);
            }
            else if constexpr ( is_same<tag, prims_mst_tag>::value )
            {
               to_visit.push_front(to_node);
            }
         }
      }
      cout << endl;
   }

public:

   void addSingleEdge(size_t from, size_t to)
   {
      addEdge_impl(from, to);
   }

   void addDoubleEdge(size_t from, size_t to)
   {
      addEdge_impl(from, to);
      addEdge_impl(to, from);
   }

   void breadth_first_search(size_t start)
   {
      traverse_impl<breadth_first_search_tag>(start);
   }

   void depth_first_search(size_t start)
   {
      traverse_impl<depth_first_search_tag>(start);
   }

   void prims()
   {
      traverse_impl<prims_mst_tag>(0);
   }

   Graph transpose()
   {
      Graph g;

      

      return g;
   }

   void kruskals();

   void dijkstras();

   void detect_cycle();
   void kosaraju();
   void bellman_ford();
   void topological_sort();
   void floyd_warshall();
};

Graph createGraph1()
{
   Graph g;

   g.addDoubleEdge(0, 1);
   g.addDoubleEdge(0, 3);
   g.addDoubleEdge(1, 3);
   g.addDoubleEdge(1, 4);
   g.addDoubleEdge(2, 3);

   return g;
}

Graph createGraph2()
{
   Graph g;
   g.addDoubleEdge(0, 1);
	g.addDoubleEdge(0, 2);
	g.addDoubleEdge(1, 2);
	g.addDoubleEdge(2, 0);
	g.addDoubleEdge(2, 3);
	g.addDoubleEdge(3, 3);
   return g;
}

int main()
{
   Graph g = createGraph1();

   g.breadth_first_search(2);
   g.depth_first_search(2);
   g.prims();

   return 0;
}