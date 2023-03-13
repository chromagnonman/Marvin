#include "Marvin.h"
#include <iostream>

namespace RobotFactory {

	struct Marvin::impl {
		void rotate(const ROTATE_DIRECTION& rotate_direction = ROTATE_DIRECTION::LEFT) noexcept;
		void move() noexcept;
	};

	void Marvin::impl::move() noexcept
	{
		// TODO:
		std::cout << "Moved\n";
	}

	void Marvin::impl::rotate(const ROTATE_DIRECTION& rotate_direction) noexcept
	{
		// TODO:
		std::cout << "RotateLeft\n";
	}

	Marvin::Marvin() noexcept :
		m_pImpl{ std::make_shared<impl>() }
	{
	}

	Marvin::Marvin(const Marvin& marvin) noexcept
	{
		if (this != &marvin)
		{
			m_pImpl = marvin.m_pImpl;
		}
	}

	Marvin& Marvin::operator=(const Marvin& marvin) noexcept
	{
		if (this != &marvin)
		{
			m_pImpl = marvin.m_pImpl;
		}

		return *this;
	}

	Marvin::~Marvin() = default;

	void Marvin::move() noexcept
	{
		m_pImpl->move();
	}

	void Marvin::rotate(const ROTATE_DIRECTION& rotate_direction) noexcept
	{
		m_pImpl->rotate(rotate_direction);
	}
}

