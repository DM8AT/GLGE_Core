/**
 * @file BVH.h
 * @author DM8AT
 * @brief define an API for a bounding volume hierarchy
 * @version 0.1
 * @date 2025-10-09
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_GEOMETRY_VOLUMES_BVH_
#define _GLGE_CORE_GEOMETRY_VOLUMES_BVH_

//include the sized types
#include "../../Types.h"

//for C++ create a class
#if __cplusplus

//include std::vectors for simple, resizable vectors
#include <vector>
//include iostream for the input output system
#include <iostream>
//include variants
#include <variant>

//if GLGE_BVH_DEBUG is defined, allow assertion. Else, do nothing. 
#ifdef GLGE_BVH_DEBUG
  #include <cassert>
  #define GLGE_BVH_ASSERT(x) assert(x)
#else
  #ifndef GLGE_BVH_ASSERT //allow custom GLGE_BVH_ASSERT implementations
    #define GLGE_BVH_ASSERT(x) ((void)0)
  #endif
#endif

/**
 * @brief define a template class for bounding volume hierarchies
 * 
 * @tparam Volume The type for the volume elements
 * @tparam Leaf the type for the element leaf
 * @tparam MaxChildCount the maximum amount of children a leaf node may have
 */
template <typename Volume, typename Leaf, uint8_t MaxChildCount = 8> class BVH {
public:
    //sanity check for the maximum child count
    static_assert(MaxChildCount > 0, "MaxChildCount must be greater than 0");

    /**
     * @brief store a single node for the BVH tree
     */
    struct Node {
        //store a combination of this volume and indices to the next m_nodes
        struct Internal {
            //bounding volume
            Volume volume;
            //indices to children
            size_t childIndices[MaxChildCount];

            //simple constructor for the internal
            Internal(const Volume& _volume)
             : volume(_volume), childIndices{} 
            {}
        };
        //store a C++ union (variant) of a leaf and an internal node
        std::variant<Leaf, Internal> data;

        //Store the amount of leafs for an internal node. 0 = leaf, >0 = internal
        uint8_t childCount = 0; 

        /**
         * @brief Construct a leaf node
         * @param leaf the leaf primitive
         */
        inline constexpr Node(const Leaf& leaf)
         : data(leaf),
           childCount(0)
        {}

        /**
         * @brief Construct an internal node
         * @param volume bounding volume
         * @param children indices to child m_nodes
         * @param count number of children
         */
        inline constexpr Node(const Volume& volume, const size_t* children, uint8_t count)
         : data(Internal(volume))
        {
            //sanity check the count
            assert(count > 0 && count <= MaxChildCount);
            //store the children count & index count
            auto& internal = std::get<Internal>(data);
            for (uint8_t i = 0; i < count; ++i) {
                internal.childIndices[i] = children[i];
            }
        }

        /**
         * @brief Checks whether this node is a leaf
         * @return true if node is a leaf
         */
        inline constexpr bool isLeaf() const noexcept {return childCount == 0;}
    };

    /**
     * @brief Construct a new BVH
     * 
     * Empty BVH
     */
    inline constexpr BVH() noexcept = default;

    /**
     * @brief Construct a new BVH
     * 
     * @param leaves the amount of leafs to create the BVH from
     * @param leafCount the amount of leafs in the C array
     */
    inline explicit BVH(const Leaf* leaves, size_t leafCount) noexcept {buildFromArray(leaves, leafCount);}

    /**
     * @brief Construct a new BVH
     * 
     * @param leaves a std::vector of leafs to create the BVH from
     */
    inline explicit BVH(const std::vector<Leaf>& leaves) noexcept {build(leaves);}

    inline const Node& getNode(size_t index) const noexcept {
        GLGE_BVH_ASSERT(index < m_nodes.size());
        return m_nodes[index];
    }

    /**
     * @brief Get a node using the node's index
     * 
     * @param index the index of the node to get
     * @return Node& a reference to the node at the requested position
     */
    inline constexpr Node& getNode(size_t index) noexcept {
        GLGE_BVH_ASSERT(index < m_nodes.size());
        return m_nodes[index];
    }

    /**
     * @brief get the amount of m_nodes (both internal m_nodes and leaf m_nodes)
     * 
     * @return constexpr size_t the amount of all m_nodes
     */
    inline constexpr size_t size() const noexcept {return m_nodes.size();}

    /**
     * @brief clear the internal structure
     */
    inline void clear() noexcept {m_nodes.clear();}

    /**
     * @brief reserve a specific amount of RAM for the internal node structure
     * 
     * @param count the amount of m_nodes to reserve RAM for
     */
    inline void reserve(size_t count) noexcept {m_nodes.reserve(count);}

    /**
     * @brief build the BVH from an std::vector of leafs
     * 
     * @param leaves the std::vector of leafs to build from
     * @return size_t the index of the root node of the BVH
     */
    inline size_t build(const std::vector<Leaf>& leaves) noexcept {return buildFromArray(leaves.data(), leaves.size());}

    /**
     * @brief build the BVH from a C array
     * 
     * @param leaves a C array containing the leaf elements to build from
     * @param leafCount the amount of leaf elements in the C array
     * @return size_t the index of the root node of the BVH
     */
    inline size_t buildFromArray(const Leaf* leaves, size_t leafCount) noexcept {
        //clean up the old BVH
        clear();

        //sanity check the leaf count
        if (leafCount == 0) {return (size_t)(-1);}

        //store the current level as well as a cache of a the node volumes
        std::vector<size_t> currentLevel;
        std::vector<Volume> volumeCache;

        //make enough space for all leafs
        currentLevel.reserve(leafCount);
        //Rough estimate: full binary tree is about 2n-1 m_nodes
        volumeCache.reserve(leafCount * 2);

        //Step 1: Create leaf m_nodes and store their volumes
        for (size_t i = 0; i < leafCount; ++i)
        {
            //create the new leaf and get the index
            size_t index = createLeaf(leaves[i]);
            //add the leaf to the current level (bottom->up, so start with leafs)
            currentLevel.push_back(index);
            //make enough size for the volume cache
            volumeCache.resize(std::max(volumeCache.size(), index + 1));
            //cache the volume
            volumeCache[index] = leafToVolume(leaves[i]);
        }

        //Step 2: Bottom-up internal node construction
        while (currentLevel.size() > 1)
        {
            //store the next level to fill out
            std::vector<size_t> nextLevel;
            //store the current index in the level
            size_t i = 0;

            //iterate over all elements in the size array
            while (i < currentLevel.size())
            {
                //calculate the size of the group
                size_t groupSize = std::min<size_t>(MaxChildCount, currentLevel.size() - i);
                //get the group to work on
                const size_t* group = currentLevel.data() + i;

                //Compute bounding volume from cached child volumes
                Volume groupVolume = computeGroupVolume(group, groupSize, volumeCache);

                //Create the internal node
                size_t nodeIndex = createInternal(groupVolume, group, static_cast<uint8_t>(groupSize));
                nextLevel.push_back(nodeIndex);

                //Store computed volume
                volumeCache.resize(std::max(volumeCache.size(), nodeIndex + 1));
                volumeCache[nodeIndex] = groupVolume;

                //increase the position in the group by the size of the current group
                i += groupSize;
            }

            //update the levels
            currentLevel = std::move(nextLevel);
        }

        //return the root node
        return currentLevel[0];
    }

    /**
     * @brief print the BVH tree into a console
     * 
     * This automatically starts a new line
     * 
     * @param os the output stream to print to
     */
    inline void print(std::ostream& os)
    {
        //sanity check if the BVH is empty
        if (m_nodes.empty()) {
            os << "(BVH is empty)\n";
            return;
        }

        //if the tree is not empty, print the tree starting at the root node
        os << "BVH Tree:\n";
        printNode(os, 0, 0);
    }

    /**
     * @brief print a node of the tree as well as all the child nodes
     * 
     * @param os the output stream to print to
     * @param index the index to start the printing at
     * @param depth the depth of elements to print. Used to set the indent correctly. 
     */
    inline void printNode(std::ostream& os, size_t index, size_t depth) noexcept
    {
        //sanity check the index
        GLGE_BVH_ASSERT(index < nodes.size());

        //get the requested node and add an indent
        Node& node = m_nodes[index];
        std::string indent(depth * 2, ' ');

        //if the node is a leaf node, print the index
        if (node.isLeaf()) {
            os << indent << "- Leaf [index=" << index << "]\n";
        } else {
            //else, print the internal node's index as well as the amount of children
            os << indent << "- Internal [index=" << index << ", children=" << int(node.childCount) << "]\n";

            //next, print all child nodes
            for (uint8_t i = 0; i < node.childCount; ++i) {
                size_t childIndex = std::get<typename Node::Internal>(node.data).childIndices[i];
                printNode(os, childIndex, depth + 1);
            }
        }
    }

protected:

    //store the internal m_nodes
    std::vector<Node> m_nodes;

    /**
     * @brief Create a Leaf new leaf
     * 
     * @param leaf the leaf to create
     * @return size_t the index of the leaf
     */
    inline size_t createLeaf(const Leaf& leaf) noexcept {
        //just add the leaf and return the index
        m_nodes.emplace_back(leaf);
        return m_nodes.size() - 1;
    }

    /**
     * @brief Create a Internal node
     * 
     * @param volume the volume of the node
     * @param children an array of child indices
     * @param count the amount of children
     * @return size_t the index of the internal node
     */
    inline size_t createInternal(const Volume& volume, const size_t* children, uint8_t count) noexcept {
        //just create the node and return the index
        m_nodes.emplace_back(volume, children, count);
        return m_nodes.size() - 1;
    }

    /**
     * @brief calculate the volume of a group combination
     * 
     * @param childIndices a pointer to a list of children
     * @param count the amount of children in the group
     * @param volumeCache a std::vector of cached volumes for the nodes
     * @return Volume the volume that contains the whole children
     */
    inline Volume computeGroupVolume(const size_t* childIndices, size_t count, const std::vector<Volume>& volumeCache) const noexcept
    {
        //sanity check the count
        GLGE_BVH_ASSERT(count > 0);

        //merge all the volumes together
        Volume merged = volumeCache[childIndices[0]];
        for (size_t i = 1; i < count; ++i)
        {merged.merge(volumeCache[childIndices[i]]);}
        //return the final merged volume
        return merged;
    }

    /**
     * @brief calculate a volume for a leaf node
     * 
     * @param leaf the leaf to calculate the volume for
     * @return Volume the volume that contains the leaf node
     */
    Volume leafToVolume(const Leaf& leaf) const noexcept
    {
        //get the bounding volume of the correct type from the leaf
        return leaf.template getBoundingVolume<Volume>();
    }

};

/**
 * @brief a template function for the printing of a bvh
 * 
 * @tparam Volume the volume type of the BVH to print
 * @tparam Leaf the leaf type of the BVH to print
 * @tparam MaxChildCount the maximum amounts of children of the BVH to print
 * @param os the output stream to print to
 * @param bvh the BVH to print
 * @return std::ostream& the filled output stream
 */
template <typename Volume, typename Leaf, uint8_t MaxChildCount>
std::ostream& operator<<(std::ostream& os, BVH<Volume, Leaf, MaxChildCount>& bvh) {
    //print the BVH to the output stream and then return it
    bvh.print(os);
    return os;
}


#endif

#endif