/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
//
// A module for create by Virtualization TLON Distributed System
// Copycenter (C) 2019
// Giuseppe Roa Osorio <groao@unal.edu.co>
// Grupo de investigación TLÖN
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser Public License for more details.
//
// You should have received a copy of the GNU Lesser Public License
// along with this program.  If not, see [http://www.gnu.org/licenses/].

//__author__ = "Giuseppe Roa Osorio"
//__copyright__ = "Copyright 2019, TLON group"
//__license__ = "LGPL"
//__version__ = "1"
//__email__ = "groao@unal.edu.co"
//__status__ = "Development"

// Include a header file from your module to test.
//#include "ns3/m2m.h"
#include "ns3/M2MAddress.h"
#include "ns3/M2MContext.h"
#include "ns3/M2MIntegrator.h"
#include "ns3/M2MPropagator.h"
#include "ns3/M2MState.h"
#include "ns3/M2MTerminal.h"

// An essential include is test.h
#include "ns3/test.h"

// Do not put your test classes in namespace ns3.  You may find it useful
// to use the using directive to access the ns3 namespace directly
using namespace ns3;

// This is an example TestCase.
class M2mTestCase1 : public TestCase
{
public:
  M2mTestCase1 ();
  virtual ~M2mTestCase1 ();

private:
  virtual void DoRun (void);
};

// Add some help text to this case to describe what it is intended to test
M2mTestCase1::M2mTestCase1 ()
  : TestCase ("M2m test case (does nothing)")
{
}

// This destructor does nothing but we include it as a reminder that
// the test case should clean up after itself
M2mTestCase1::~M2mTestCase1 ()
{
}

//
// This method is the pure virtual method from class TestCase that every
// TestCase must implement
//
void
M2mTestCase1::DoRun (void)
{
  // A wide variety of test macros are available in src/core/test.h
  NS_TEST_ASSERT_MSG_EQ (true, true, "true doesn't equal true for some reason");
  // Use this one for floating point comparisons
  NS_TEST_ASSERT_MSG_EQ_TOL (0.01, 0.01, 0.001, "Numbers are not equal within tolerance");
}

// The TestSuite class names the TestSuite, identifies what type of TestSuite,
// and enables the TestCases to be run.  Typically, only the constructor for
// this class must be defined
//
class M2mTestSuite : public TestSuite
{
public:
  M2mTestSuite ();
};

M2mTestSuite::M2mTestSuite ()
  : TestSuite ("m2m", UNIT)
{
  // TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
  AddTestCase (new M2mTestCase1, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static M2mTestSuite m2mTestSuite;

