/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2016 Jan Haller
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef THOR_CONCAVESHAPE_HPP
#define THOR_CONCAVESHAPE_HPP

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/NonCopyable.hpp>

#include <array>
#include <cassert>
#include <memory>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <iterator>
#include <type_traits>
#include <vector>
#ifndef NDEBUG
 #include <typeinfo>
#endif

namespace thor
{

namespace detail
{

	bool isClockwiseOriented(sf::Vector2f v0, sf::Vector2f v1, sf::Vector2f v2);

	template <typename V>
	sf::Vector2f getVertexPosition(const V& vertex);

} // namespace detail

template <typename V>
class Edge
{
	public:
		Edge(V& corner0, V& corner1)
        {
            assert(detail::getVertexPosition(corner0) != detail::getVertexPosition(corner1));
            mCorners[0] = &corner0;
            mCorners[1] = &corner1;
        }

		V& operator[] (std::size_t cornerIndex) { return *mCorners[cornerIndex]; }
		const V& operator[] (std::size_t cornerIndex) const { return *mCorners[cornerIndex]; }

	protected:
		std::array<V*, 2> mCorners;
};

template <typename V>
class Triangle
{
	public:
        Triangle(V& corner0, V& corner1, V& corner2)
        {
            assert(detail::isClockwiseOriented(detail::getVertexPosition(corner0), detail::getVertexPosition(corner1), detail::getVertexPosition(corner2)));
            mCorners[0] = &corner0;
            mCorners[1] = &corner1;
            mCorners[2] = &corner2;
        }

		V& operator[] (std::size_t cornerIndex) { return *mCorners[cornerIndex]; }
		const V& operator[] (std::size_t cornerIndex) const { return *mCorners[cornerIndex]; }

	private:
		std::array<V*, 3> mCorners;
};

template <typename V>
struct TriangulationTraits
{
	static sf::Vector2f getPosition(const V& vertex)
	{
		return vertex.getPosition();
	}
};

template <>
struct TriangulationTraits<sf::Vector2f>
{
	static sf::Vector2f getPosition(sf::Vector2f vertex)
	{
		return vertex;
	}
};

namespace detail
{
	class AdvancedTriangle;
	class AdvancedVertex;
	class AdvancedEdge;

	struct OptTriangleIterator;
	struct OptTriangleItrArray;

	struct  CompareVertexPtrs
	{
		bool operator() (const AdvancedVertex* lhs, const AdvancedVertex* rhs) const;
	};

	struct  CompareEdges
	{
		bool operator() (const AdvancedEdge& lhs, const AdvancedEdge& rhs) const;
	};
	typedef std::set<AdvancedVertex*, CompareVertexPtrs>	VertexPtrSet;
	typedef VertexPtrSet::iterator							VertexPtrIterator;

	class  AdvancedVertex
	{
		public:
			template <typename V>
										AdvancedVertex(V& userVertex, OptTriangleIterator surroundingTriangle);
										AdvancedVertex(float x, float y);

			sf::Vector2f				getPosition() const;
			void						setSurroundingTriangle(OptTriangleIterator target);
			OptTriangleIterator			getSurroundingTriangle() const;

			template <typename V>
			V&							getUserVertex() const;

		private:
			void*									mUserVertex;
			sf::Vector2f							mPosition;
			std::shared_ptr<OptTriangleIterator>	mSurroundingTriangle;
#ifndef NDEBUG
			const std::type_info*					mUserType;
#endif
	};

	class  AdvancedEdge : public Edge<AdvancedVertex>
	{
		public:
										AdvancedEdge(AdvancedVertex& startPoint, AdvancedVertex& endPoint);

		private:
			void						orderCorners();
	};

	class  AdvancedTriangle : public Triangle<AdvancedVertex>
	{
		public:
										AdvancedTriangle(AdvancedVertex& corner0, AdvancedVertex& corner1, AdvancedVertex& corner2);

			void						addVertex(AdvancedVertex& vertexRef);
			void						removeVertex(AdvancedVertex& vertexRef);
			void						removeVertex(VertexPtrIterator vertexItr);
			VertexPtrIterator			begin();
			VertexPtrIterator			end();
			void						setAdjacentTriangle(std::size_t index, const OptTriangleIterator& adjacentTriangle);
			OptTriangleIterator			getAdjacentTriangle(std::size_t index) const;
			void						setFlagged(bool flagged);
			bool						isFlagged() const;

		private:
			VertexPtrSet							mRemainingVertices;
			std::shared_ptr<OptTriangleItrArray>	mAdjacentTriangles;
			bool									mFlagged;
	};

	typedef std::deque<AdvancedVertex>						VertexCtr;
	typedef std::set<AdvancedEdge, CompareEdges>			EdgeSet;
	typedef std::list<AdvancedTriangle>						TriangleList;

	typedef TriangleList::iterator							TriangleIterator;

	struct  OptTriangleIterator
	{
									OptTriangleIterator();
									OptTriangleIterator(TriangleIterator target);
									OptTriangleIterator(const OptTriangleIterator& origin);
		OptTriangleIterator&		operator= (const OptTriangleIterator& origin);

		bool						valid;
		TriangleIterator			target;
	};

	struct Circle
	{
        Circle(sf::Vector2f midPoint, float squaredRadius);
		sf::Vector2f midPoint;
		float squaredRadius;
	};

	template <typename T>
	struct DereferencedIterator
	{
		typedef typename std::remove_pointer<typename std::iterator_traits<T>::pointer>::type value_type;
	};

	template <typename V>
	AdvancedVertex::AdvancedVertex(V& userVertex, OptTriangleIterator surroundingTriangle)
	: mUserVertex(const_cast<typename std::remove_const<V>::type*>(&userVertex))
	, mPosition(getVertexPosition(userVertex))
	, mSurroundingTriangle(std::make_shared<OptTriangleIterator>(surroundingTriangle))
#ifndef NDEBUG
	, mUserType(&typeid(V))
#endif
	{
	}

	template <typename V>
	V& AdvancedVertex::getUserVertex() const
	{
		assert(typeid(V) == *mUserType);
		return *static_cast<V*>(mUserVertex);
	}

	void insertPoint(TriangleList& triangles, AdvancedVertex& vertex, const AdvancedTriangle& boundaryTriangle, const EdgeSet& constrainedEdges);
	void removeUnusedTriangles(TriangleList& triangles, const AdvancedTriangle& boundaryTriangle, const EdgeSet& constrainedEdges, bool limitToPolygon);
	void createBoundaryPoints(VertexCtr& allVertices, TriangleList& triangles);
	void setBoundaryPositions(const VertexCtr& allVertices, AdvancedTriangle& boundaryTriangle);

	template <typename InputIterator>
	struct ConstrainedTrDetails
	{
		ConstrainedTrDetails(InputIterator constrainedEdgesBegin, InputIterator constrainedEdgesEnd)
		: constrainedEdgesBegin(constrainedEdgesBegin)
		, constrainedEdgesEnd(constrainedEdgesEnd)
		{
		}
		InputIterator constrainedEdgesBegin;
		InputIterator constrainedEdgesEnd;
		static const bool isPolygon = false;
	};

	struct PolygonTrDetails
	{
		static const bool isPolygon = true;
	};

	template <typename OutputIterator, typename UserVertex>
	struct PolygonOutputTrDetails
	{
		PolygonOutputTrDetails(OutputIterator edgesOut)
		: edgesOut(edgesOut)
		{
		}
		OutputIterator edgesOut;
		static const bool isPolygon = true;
	};

	template <typename V>
	sf::Vector2f getVertexPosition(const V& vertex)
	{
		return TriangulationTraits<V>::getPosition(vertex);
	}

	template <typename InputIterator1, typename InputIterator2>
	void collateVerticesConstrained(TriangleIterator firstTriangle, VertexCtr& allVertices, EdgeSet& constrainedEdges, InputIterator1 verticesBegin, InputIterator1 verticesEnd, InputIterator2 constrainedEdgesBegin, InputIterator2 constrainedEdgesEnd)
	{
		typedef typename DereferencedIterator<InputIterator1>::value_type UserVertex;
		typedef typename std::iterator_traits<InputIterator2>::value_type UserEdge;
		typedef std::set<UserVertex*> RawVertexPtrSet;
		RawVertexPtrSet importantVertices;
		for (InputIterator2 itr = constrainedEdgesBegin; itr != constrainedEdgesEnd; ++itr)
		{
			UserEdge& edge = *itr;
			importantVertices.insert(&edge[0]);
			importantVertices.insert(&edge[1]);
		}
		std::map<UserVertex*, AdvancedVertex*> map;
		for (UserVertex* userVertex : importantVertices)
		{
			allVertices.push_back(AdvancedVertex(*userVertex, firstTriangle));
			AdvancedVertex& advancedVertex = allVertices.back();
			firstTriangle->addVertex(advancedVertex);
			map.insert(std::make_pair(userVertex, &advancedVertex));
		}
		for (InputIterator2 itr = constrainedEdgesBegin; itr != constrainedEdgesEnd; ++itr)
		{
			UserEdge& userEdge = *itr;
			AdvancedEdge advancedEdge(*map[&userEdge[0]], *map[&userEdge[1]]);
			constrainedEdges.insert(advancedEdge);
		}
		for (; verticesBegin != verticesEnd; ++verticesBegin)
		{
			UserVertex& userVertex = *verticesBegin;
			if (importantVertices.find(&userVertex) == importantVertices.end())
			{
				allVertices.push_back(detail::AdvancedVertex(userVertex, firstTriangle));
				firstTriangle->addVertex(allVertices.back());
			}
		}
	}

	inline void addEdge(EdgeSet& constrainedEdges, AdvancedVertex* previousVertex, AdvancedVertex& currentVertex, PolygonTrDetails)
	{
		if (previousVertex != nullptr)
			constrainedEdges.insert(AdvancedEdge(*previousVertex, currentVertex));
	}

	template <typename OutputIterator, typename UserVertex>
	void addEdge(EdgeSet& constrainedEdges, AdvancedVertex* previousVertex, AdvancedVertex& currentVertex, PolygonOutputTrDetails<OutputIterator, UserVertex> details)
	{
		if (previousVertex != nullptr)
		{
			*details.edgesOut++ = Edge<UserVertex>(previousVertex->getUserVertex<UserVertex>(), currentVertex.getUserVertex<UserVertex>());
			constrainedEdges.insert(AdvancedEdge(*previousVertex, currentVertex));
		}
	}

	template <typename InputIterator, class AdditionalDetails>
	void collateVerticesPolygon(TriangleIterator firstTriangle, VertexCtr& allVertices, EdgeSet& constrainedEdges, InputIterator verticesBegin, InputIterator verticesEnd, const AdditionalDetails& details)
	{
		if (verticesBegin == verticesEnd)
			return;
		AdvancedVertex* previousVertex = nullptr;
		for (; verticesBegin != verticesEnd; ++verticesBegin)
		{
			allVertices.push_back(AdvancedVertex(*verticesBegin, firstTriangle));
			AdvancedVertex& vertex = allVertices.back();
			firstTriangle->addVertex(vertex);
			addEdge(constrainedEdges, previousVertex, vertex, details);
			previousVertex = &vertex;
		}
		AdvancedVertex& firstVertex = allVertices[3];
		addEdge(constrainedEdges, previousVertex, firstVertex, details);
	}

	template <typename InputIterator1, typename InputIterator2>
	void collateVertices(TriangleIterator firstTriangle, VertexCtr& allVertices, EdgeSet& constrainedEdges, InputIterator1 verticesBegin, InputIterator1 verticesEnd, const ConstrainedTrDetails<InputIterator2>& details)
	{
		collateVerticesConstrained(firstTriangle, allVertices, constrainedEdges, verticesBegin, verticesEnd, details.constrainedEdgesBegin, details.constrainedEdgesEnd);
	}

	template <typename InputIterator>
	void collateVertices(TriangleIterator firstTriangle, VertexCtr& allVertices, EdgeSet& constrainedEdges, InputIterator verticesBegin, InputIterator verticesEnd, const PolygonTrDetails& details)
	{
		collateVerticesPolygon(firstTriangle, allVertices, constrainedEdges, verticesBegin, verticesEnd, details);
	}

	template <typename InputIterator, typename OutputIterator, typename UserVertex>
	void collateVertices(TriangleIterator firstTriangle, VertexCtr& allVertices, EdgeSet& constrainedEdges, InputIterator verticesBegin, InputIterator verticesEnd, const PolygonOutputTrDetails<OutputIterator, UserVertex>& details)
	{
		collateVerticesPolygon(firstTriangle, allVertices, constrainedEdges, verticesBegin, verticesEnd, details);
	}

	template <typename UserVertex, typename InputIterator, typename OutputIterator>
	OutputIterator transformTriangles(InputIterator begin, InputIterator end, OutputIterator out)
	{
		for (; begin != end; ++begin)
		{
			const AdvancedTriangle& current = *begin;
			*out++ = Triangle<UserVertex>( current[0].getUserVertex<UserVertex>(), current[1].getUserVertex<UserVertex>(),current[2].getUserVertex<UserVertex>());
		}
		return out;
	}

	template <typename InputIterator, typename OutputIterator, class AdditionalDetails>
	OutputIterator triangulateImpl(InputIterator verticesBegin, InputIterator verticesEnd, OutputIterator trianglesOut, const AdditionalDetails& details)
	{
		VertexCtr allVertices;
		TriangleList triangles;
		EdgeSet constrainedEdges;createBoundaryPoints(allVertices, triangles);
		AdvancedTriangle boundaryTriangle = triangles.front();
		collateVertices(triangles.begin(), allVertices, constrainedEdges, verticesBegin, verticesEnd, details);
		setBoundaryPositions(allVertices, boundaryTriangle);
		for (VertexCtr::iterator itr = allVertices.begin() + 3, end = allVertices.end(); itr != end; ++itr)
			insertPoint(triangles, *itr, boundaryTriangle, constrainedEdges);
		removeUnusedTriangles(triangles, boundaryTriangle, constrainedEdges, AdditionalDetails::isPolygon);
		typedef typename DereferencedIterator<InputIterator>::value_type UserVertex;
		return transformTriangles<UserVertex>(triangles.begin(), triangles.end(), trianglesOut);
	}

} // namespace detail

template <typename InputIterator, typename OutputIterator>
OutputIterator triangulate(InputIterator verticesBegin, InputIterator verticesEnd, OutputIterator trianglesOut)
{
	typedef typename detail::DereferencedIterator<InputIterator>::value_type UserVertex;
	std::vector<Edge<UserVertex>> noEdges;
	return triangulateConstrained(verticesBegin, verticesEnd, noEdges.begin(), noEdges.end(), trianglesOut);
}

template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator triangulateConstrained(InputIterator1 verticesBegin, InputIterator1 verticesEnd,
	InputIterator2 constrainedEdgesBegin, InputIterator2 constrainedEdgesEnd, OutputIterator trianglesOut)
{
	return detail::triangulateImpl(verticesBegin, verticesEnd, trianglesOut, detail::ConstrainedTrDetails<InputIterator2>(constrainedEdgesBegin, constrainedEdgesEnd));
}

template <typename InputIterator, typename OutputIterator>
OutputIterator triangulatePolygon(InputIterator verticesBegin, InputIterator verticesEnd, OutputIterator trianglesOut)
{
	return detail::triangulateImpl(verticesBegin, verticesEnd, trianglesOut, detail::PolygonTrDetails());
}

template <typename InputIterator, typename OutputIterator1, typename OutputIterator2>
OutputIterator1 triangulatePolygon(InputIterator verticesBegin, InputIterator verticesEnd, OutputIterator1 trianglesOut, OutputIterator2 edgesOut)
{
	return detail::triangulateImpl(verticesBegin, verticesEnd, trianglesOut, detail::PolygonOutputTrDetails<OutputIterator2, typename detail::DereferencedIterator<InputIterator>::value_type>(edgesOut));
}

} // namespace thor

namespace thor
{

/// @addtogroup Shapes
/// @{

/// @brief Concave shape class
/// @details This class has an interface and functionality similar to sf::ConvexShape, but is additionally able to work
///  with shapes that are concave. It inherits the sf::Drawable and sf::Transformable classes.
class ConcaveShape : public sf::Drawable, public sf::Transformable
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Creates an empty shape.
		/// @details You can use setPointCount() and SetPoint() to build your own concave shape.
									ConcaveShape();

		/// @brief Implicit constructor that adapts an existing sf::Shape.
		/// @details The concave shape should look as similar as possible to the original convex shape.
									ConcaveShape(const sf::Shape& shape);

		/// @brief Sets the amount of points in the concave polygon.
		///
		void						setPointCount(std::size_t count);

		/// @brief Returns the amount of points in the concave polygon.
		///
		std::size_t					getPointCount() const;

		/// @brief Sets the position of a point.
		/// @param index Which point? Must be in [0, getPointCount()[
		/// @param position New point position in local coordinates.
		void						setPoint(std::size_t index, sf::Vector2f position);

		/// @brief Returns the position of a point.
		/// @param index Which point? Must be in [0, getPointCount()[
		sf::Vector2f				getPoint(std::size_t index) const;

		/// @brief Sets the polygon's fill color.
		///
		void						setFillColor(const sf::Color& fillColor);

		/// @brief Sets the polygon's outline color.
		///
		void						setOutlineColor(const sf::Color& outlineColor);

		/// @brief Returns the polygon's fill color.
		///
		sf::Color					getFillColor() const;

		/// @brief Returns the polygon's outline color.
		///
		sf::Color					getOutlineColor() const;

		/// @brief sets the thickness of the shape's outline (0.f by default).
		/// @details @a outlineThickness must be greater or equal to zero. If it is zero, no outline is visible.
		void						setOutlineThickness(float outlineThickness);

		/// @brief Returns the shape's outline thickness.
		///
		float						getOutlineThickness() const;

		/// @brief Return untransformed bounding rectangle.
		///
		sf::FloatRect				getLocalBounds() const;

		/// @brief Return transformed bounding rectangle.
		/// @details This function does not necessarily yield the minimal bounding rect that spans all vertices. It transforms
		///  the local bounding rect, and then computes the bounding rect of that transformed bounding rect -- so for example, a
		///  rotated shape might not have the bounding rect that you expect. This behavior is consistent with SFML.
		sf::FloatRect				getGlobalBounds() const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private types
	private:
		struct TriangleGenerator;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private member functions
	private:
		// Renders the shape to target.
		virtual void				draw(sf::RenderTarget& target, sf::RenderStates states) const;

		// Computes how the shape can be split up into convex triangles.
		void						ensureDecomposed() const;

		// Forms the outline out of the given edges.
		void						ensureOutlineUpdated() const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		std::vector<sf::Vector2f>				mPoints;
		sf::Color								mFillColor;
		sf::Color								mOutlineColor;
		float									mOutlineThickness;

		mutable sf::VertexArray					mTriangleVertices;
		mutable std::vector<sf::ConvexShape>	mOutlineShapes;
		mutable sf::FloatRect					mLocalBounds;
		mutable bool							mNeedsDecomposition;
		mutable bool							mNeedsOutlineUpdate;
};

/// @}

} // namespace thor

#endif // THOR_CONCAVESHAPE_HPP
