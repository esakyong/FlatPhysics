#pragma once

namespace FlatPhysics
{
	struct FlatAABB;

	class FlatIAABB
	{
	public:
		virtual ~FlatIAABB() = default;
		virtual FlatAABB getAABB() const = 0;
	};
}

