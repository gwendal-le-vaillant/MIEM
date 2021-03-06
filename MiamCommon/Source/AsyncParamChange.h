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

#ifndef ASYNCPARAMCHANGE_H_INCLUDED
#define ASYNCPARAMCHANGE_H_INCLUDED

#include <string>
#include "boost/lexical_cast.hpp"

#include "ParametersIds.h"


namespace Miam
{
    /// \brief A simple structure employed to send messages between the
    /// Miam::Presenter and the Miam::Model (in one way or another) via
    /// a boost lock-free queue
    struct AsyncParamChange
    {
        /// \brief The following parameter types are quite generic,
        /// and they can be detailed using the IDs from ParametersIds.h
        ///
        /// The IDs from ParametersIds.h can be stored within the
        /// Id1, Id2 integer atributes.
        ///
        /// Insert new values juste before the fixed ones (100, 200, etc...)
        /// because the int values are also employed for TCP interprocess
        /// communication.
        enum ParamType {
            
            None = 0,
            
            Reinitialize,
            LoadingComplete,
            
            
            
            Play = 100, ///< Play command
                Playing, ///< Play information
            Pause,
            Stop, ///< Stop command
                Stopped, ///< Stop information
            
            
            Activate = 200,
            MasterVolume,
            Volume,
                Volume_CorrelatedInputs,
                Volume_DecorrelatedInputs,
            Frequency,
            Excitement,
            Duration,
            
            
            Position = 300,
            Source,
            
            
            InputsCount = 400,
            OutputsCount,
            InputsAndOutputsCount,
            
            
            
            Update = 500,
            UpdateDisplay,
            
            
            
            UdpPort = 600,
            TcpPort,
            
            
            Scene = 700,
            SceneTransitionBegan, ///< The scene is being changed (values might be incoherent)
            SceneTransitionEnded, ///< The new scene is fully loaded, values are usable
            
            
            LastEnumValue ///< Last, exclusive (non-usable) value. Must remain at the very end of the enum.
        };
        /// \brief Basic, incomplete test !!!!! MUST REFACTOR TO ENUM CLASS ?????
        /// Or we must write a full new test function....
        static bool IsIntValidParamType(int intParamType)
        {
            return (0 <= intParamType) && (intParamType < ParamType::LastEnumValue);
        }
        
        
        public :
        
        /// \brief Default constructor with init to null values.
        AsyncParamChange() {Reset();}
        
        /// \brief Constructor with init of the type of parameter only
        AsyncParamChange(ParamType paramType)
        {
            Reset();
            Type = paramType;
        }
        
        
        ParamType Type = ParamType::None;
        
        int Id1, Id2;
        
        double DoubleValue;
        int IntegerValue;
        float FloatValue;
        
        void Reset()
        {
            Type = ParamType::None;
            
            Id1 = 0;
            Id2 = 0;
            
            DoubleValue = 0.0;
            IntegerValue = 0;
            FloatValue = 0.0f;
        }
        
        std::string ToString()
        {
            std::string returnString = "ParamChange : ";
            
            switch(Type)
            {
                case None: returnString = "None"; break;
                case Volume : returnString = "Volume"; break;
                    
                default :
                    returnString = boost::lexical_cast<std::string>((int)Type);
            }
            
            returnString += " Id1=";
            returnString += boost::lexical_cast<std::string>(Id1);
            returnString += " Id2=";
            returnString += boost::lexical_cast<std::string>(Id2);
            returnString += " DoubleValue=";
            returnString += boost::lexical_cast<std::string>(DoubleValue);
            returnString += " IntegerValue=";
            returnString += boost::lexical_cast<std::string>(IntegerValue);
            
            return returnString;
        }
        
        
    };
    
}


#endif  // ASYNCPARAMCHANGE_H_INCLUDED
