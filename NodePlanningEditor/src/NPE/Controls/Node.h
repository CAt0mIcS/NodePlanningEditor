#pragma once

#include "pch.h"
#include "Control.h"

#include "NPE/Util/Props.h"


namespace NPE
{
	class MainWindow;

	class Node : public Control
	{
	public:
		Node(MainWindow* parent, const NPoint pos, const NSize size);

	private:
	};
}



