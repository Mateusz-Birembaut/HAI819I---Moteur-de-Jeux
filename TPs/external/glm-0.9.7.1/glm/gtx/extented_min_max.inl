///////////////////////////////////////////////////////////////////////////////////
/// OpenGL Mathematics (glm.g-truc.net)
///
/// Copyright (c) 2005 - 2015 G-Truc Creation (www.g-truc.net)
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
/// 
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
/// 
/// Restrictions:
///		By making use of the Software for military purposes, you choose to make
///		a Bunny unhappy.
/// 
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///
/// @ref gtx_extented_min_max
/// @file glm/gtx/extented_min_max.inl
/// @date 2007-03-14 / 2011-06-07
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////////////////////

namespace glm
{
	template <typename T>
	GLM_FUNC_QUALIFIER T min(
		T const & x, 
		T const & y, 
		T const & z)
	{
		return glm::localMin(glm::localMin(x, y), z);
	}

	template <typename T, template <typename> class C>
	GLM_FUNC_QUALIFIER C<T> min
	(
		C<T> const & x, 
		typename C<T>::T const & y, 
		typename C<T>::T const & z
	)
	{
		return glm::localMin(glm::localMin(x, y), z);
	}

	template <typename T, template <typename> class C>
	GLM_FUNC_QUALIFIER C<T> min
	(
		C<T> const & x, 
		C<T> const & y, 
		C<T> const & z
	)
	{
		return glm::localMin(glm::localMin(x, y), z);
	}

	template <typename T>
	GLM_FUNC_QUALIFIER T min
	(
		T const & x, 
		T const & y, 
		T const & z, 
		T const & w
	)
	{
		return glm::localMin(glm::localMin(x, y), glm::localMin(z, w));
	}

	template <typename T, template <typename> class C>
	GLM_FUNC_QUALIFIER C<T> min
	(
		C<T> const & x, 
		typename C<T>::T const & y, 
		typename C<T>::T const & z, 
		typename C<T>::T const & w
	)
	{
		return glm::localMin(glm::localMin(x, y), glm::localMin(z, w));
	}

	template <typename T, template <typename> class C>
	GLM_FUNC_QUALIFIER C<T> min
	(
		C<T> const & x, 
		C<T> const & y, 
		C<T> const & z, 
		C<T> const & w
	)
	{
		return glm::localMin(glm::localMin(x, y), glm::localMin(z, w));
	}

	template <typename T>
	GLM_FUNC_QUALIFIER T max(
		T const & x, 
		T const & y, 
		T const & z)
	{
		return glm::max(glm::max(x, y), z);
	}

	template <typename T, template <typename> class C>
	GLM_FUNC_QUALIFIER C<T> max
	(
		C<T> const & x, 
		typename C<T>::T const & y, 
		typename C<T>::T const & z
	)
	{
		return glm::max(glm::max(x, y), z);
	}

	template <typename T, template <typename> class C>
	GLM_FUNC_QUALIFIER C<T> max
	(
		C<T> const & x, 
		C<T> const & y, 
		C<T> const & z
	)
	{
		return glm::max(glm::max(x, y), z);
	}

	template <typename T>
	GLM_FUNC_QUALIFIER T max
	(
		T const & x, 
		T const & y, 
		T const & z, 
		T const & w
	)
	{
		return glm::max(glm::max(x, y), glm::max(z, w));
	}

	template <typename T, template <typename> class C>
	GLM_FUNC_QUALIFIER C<T> max
	(
		C<T> const & x, 
		typename C<T>::T const & y, 
		typename C<T>::T const & z, 
		typename C<T>::T const & w
	)
	{
		return glm::max(glm::max(x, y), glm::max(z, w));
	}

	template <typename T, template <typename> class C>
	GLM_FUNC_QUALIFIER C<T> max
	(
		C<T> const & x, 
		C<T> const & y, 
		C<T> const & z, 
		C<T> const & w
	)
	{
		return glm::max(glm::max(x, y), glm::max(z, w));
	}

}//namespace glm
