// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * myDsl.hpp
 * Copyright (C) Cátedra SAES-UMU 2010 <andres.senac@um.es>
 *
 * EMF4CPP is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EMF4CPP is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MYDSL_HPP
#define MYDSL_HPP

#include "myDsl_forward.hpp"
#include <ecorecpp/mapping.hpp>

// EPackage


// EClass
#include "myDsl/Model.hpp"
#include "myDsl/Import.hpp"
#include "myDsl/Type.hpp"
#include "myDsl/SimpleType.hpp"
#include "myDsl/Entity.hpp"
#include "myDsl/Property.hpp"

// Package & Factory
#include "myDsl/MyDslPackage.hpp"
#include "myDsl/MyDslFactory.hpp"

#endif // MYDSL_HPP
