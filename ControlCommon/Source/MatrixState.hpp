/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2019 Gwendal Le Vaillant.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <memory>

#include "ControlState.hpp"
#include "AudioDefines.h"

#include "ControlMatrix.hpp"

#include "MiamExceptions.h"

namespace Miam
{
    
    /// \brief Low-level interpolable Routing matrix
    template<typename T>
    class MatrixState : public ControlState<T>
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        // Warning : copy-constructor is explicit in this class
        protected :
        
        // La base : la matrice creuse cachée à l'intérieur de l'état
        ControlMatrix<T> matrix;
        
        /// \brief List of values (1D indexes, not actual 2D matrix indexes)
        /// that should be the default value. Default values are not stored
        /// within this states,
        /// but if default values are used, then the actual matrix coefficients must be set to these
        /// exact default values.
        std::vector<size_t> defaultValuesIndexes;
        
        // Attribut dupliqués depuis l'interpolateur
        int inputsCount = 0;
        int outputsCount = 0;
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        
        size_t GetInputsCount()
        {return (size_t) inputsCount;}
        size_t GetOutputsCount() override
        {return (size_t) outputsCount;}

        /// \brief Returns a reference to the const (immutable) internal ControlMatrix
        ///
        /// The internal ControlMatrix is not actually immutable, but the non-const functions of
        /// the internal ControlMatrix are not all accessible from this class.
        ///
        /// \returns A const pointer to a const ControlMatrix object
        inline const ControlMatrix<T> * const GetMatrix() {return &matrix;}
        
        /// \brief Accès aux cases de la matrice par opérateur [] à simple entrée
        inline T operator[] (size_t k) const { return matrix[k]; }

        /// \brief Accès aux cases de la matrice par opérateur () à double entrée
        ///
        /// Opérateur [] n'accepte pas la surcharge à 2 paramètres
        inline T operator() (size_t i, size_t j) const { return matrix(i,j); }
        
        /// \brief Write access to the internal sparse matrix
        inline void SetValue(size_t i, size_t j, T value) { matrix.Set(i, j, value); }
        
        std::vector<size_t> GetDefaultValues() { return defaultValuesIndexes; }
        void SetDefaultValues(std::vector<size_t> _defaultValuesIndexes)
        { defaultValuesIndexes = _defaultValuesIndexes; } // full-vector copy....

        
#ifdef __MIAM_DEBUG
        void DisplayMatrixInStdCout()
        {
            std::cout << "[MatrixState.hpp] " << matrix.GetNonZeroCoeffsCount() << " non-zero coeffs: ";
            matrix.DisplayInStdCout();
            std::cout << std::endl;
        }
#endif
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        
        // - - - - - Construction / destruction - - - - -
        
        /// \brief Builds a state based on a zero Miam::SparseMatrix
        MatrixState()
        {
            this->SetName("Matrix-based generic control state");
        }
        /// \brief Explicit copy constructor
        ///
        /// Reference cannot be const because of the internal copy-constructed sparse matrix
        MatrixState(MatrixState<T> & matrixStateToCopy)
        :
        ControlState<T>(matrixStateToCopy),
        matrix( matrixStateToCopy.matrix ), // access to private member !
        inputsCount( matrixStateToCopy.inputsCount ),
        outputsCount( matrixStateToCopy.outputsCount )
        {
        }
        virtual ~MatrixState() {}
        
        
        // - - - - - Output channels (speakers) : add, delete, swap, ... - - - - -
        
        void AddOutput() override
        {throw std::logic_error("MIEM not implemented");}
        void DeleteOutput(size_t /*i*/) override
        {throw std::logic_error("MIEM not implemented");}
        void SwapOutputs(size_t /*i*/, size_t /*j*/) override
        {throw std::logic_error("MIEM not implemented");}
        
        // - - - - - Input channels : add, delete, swap, ... - - - - -

        /// \brief Does nothing.... Static matrix for now (keeps all existing coeffs)
        void SetInputOutputChannelsCount(int inputsCount_, int outputsCount_)
        {
            inputsCount = inputsCount_;
            outputsCount = outputsCount_;
        }
        bool IsIndexWithinActualInputOutputBounds(size_t index1d)
        {
            auto index2d = matrix.GetIndex2dFromIndex(index1d);
            return ((int)(index2d.i) < inputsCount)
            && ((int)(index2d.j) < outputsCount);
        }

        // - - - - - Matrix management - - - - -
        public :
        
        void ClearMatrix() { matrix.Clear(); }
        
        /// \brief Copy-constructs a duplicate of the internal matrix
        ///
        /// Dynamically allocates memory !
        std::shared_ptr< ControlMatrix<T> > GetMatrixCopy()
        { return std::make_shared< ControlMatrix<T> >(matrix); }
        
        /// \brief Internally sets the matrix from a shared_ptr of another
        void SetMatrix(std::shared_ptr< ControlMatrix<T> > newMatrix)
        {
            matrix = *(newMatrix.get());
        }
        
        /// \brief Surcharge sans 'const' car la matrice a des attributs modifiés
        /// quand on la lit (conteneur optimisé...)
        void operator+=(MatrixState<T>& matrixState)
        {
            matrix += matrixState.matrix;
        }
        
        /// \brief See Miam::SparseMatrix<T>::MultiplyAndAccumulate
        void MultiplyAndAccumulate(MatrixState<T>& matrixToMultAndAdd, T factor)
        {
            matrix.MultiplyAndAccumulate(matrixToMultAndAdd.matrix, factor);
        }
        /// \brief See Miam::SparseMatrix<T>::MultiplyByFactor
        void MultiplyByFactor(T factor)
        {
            matrix.MultiplyByFactor(factor);
        }
        
        T ComputeMatrixTotalVolume(CorrelationLevel inputCorrelationLevel, CorrelationLevel outputCorrelationLevel)
        {
            return matrix.ComputeTotalVolume(inputCorrelationLevel, outputCorrelationLevel,
                                             0, 0,
                                             inputsCount, outputsCount);
        }
        
        // - - - - - Property tree (for XML) import/export - - - - -
        virtual std::shared_ptr<bptree::ptree> GetTree() override
        {
            // Generic control data
            auto pTree = this->ControlState<T>::GetTree();
            // Matrix coefficients
            pTree->put_child("matrix", *(matrix.GetTree()));
            // Indexes of Default values
            if (defaultValuesIndexes.size() > 0)
            {
                bptree::ptree defaultTree;
                for (size_t i=0 ; i<defaultValuesIndexes.size() ; i++)
                {
                    bptree::ptree coeffTree;
                    coeffTree.put("<xmlattr>.row", defaultValuesIndexes[i]);
                    defaultTree.add_child("coeff", coeffTree);
                }
                pTree->put_child("default_values", defaultTree);
            }
            return pTree;
        }
        
        virtual void SetFromTree(bptree::ptree & stateTree) override
        {
            // At first, common properties for all states
            this->ControlState<T>::SetFromTree(stateTree);
            
            // Then, matrix attributes. SparseMatrix will
            // reinit at first (which may not be optimal when called just
            // after construction... But the compiler could see it)
            try {
                auto matrixTree = stateTree.get_child("matrix");
                matrix.SetFromTree(matrixTree);
            }
            catch (bptree::ptree_bad_path& e) {
                throw XmlReadException::FromBptree("state", e);
            }
            // default values (if exist) (not  mandatory, for retro-compatibility)
            defaultValuesIndexes.clear();
            try {
                auto defaultTree = stateTree.get_child("default_values");
                
                for(bptree::ptree::value_type &childTree : defaultTree.get_child(""))
                {
                    if (childTree.first == "coeff")
                    {
                        // Properties reading from tree, and checking
                        auto coeffTree = childTree.second;
                        size_t row = coeffTree.get<size_t>("<xmlattr>.row"); // no check on row...
                        // if tag exists, is means "default == true, for this row"
                        defaultValuesIndexes.push_back(row);
                    }
                }
            }
            catch (bptree::ptree_bad_data &e) { // Parse error: we actually treat it
                XmlReadException::FromBptree("default_values.coeff, data parse error: ", e);
            }
            catch (bptree::ptree_bad_path& /*e*/) { // bad path tolerated (compatibility with older versions)
            }
        }
        
    };
    
}

