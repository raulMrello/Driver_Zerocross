/*
 * Zerocross.cpp
 *
 *  Created on: 20/04/2015
 *      Author: raulMrello
 */

#include "Zerocross.h"



//------------------------------------------------------------------------------------
//--- PRIVATE TYPES ------------------------------------------------------------------
//------------------------------------------------------------------------------------
static const char* _MODULE_ = "[Zerocross].....";
#define _EXPR_	(_defdbg && !IS_ISR())


static void defaultNullCallback(Zerocross::LogicLevel level){
}
 
//------------------------------------------------------------------------------------
//-- PUBLIC METHODS IMPLEMENTATION ---------------------------------------------------
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
Zerocross::Zerocross(PinName gpio_zc, bool defdbg) : _defdbg(defdbg){
    // Crea objeto con valores por defecto
	DEBUG_TRACE_I(_EXPR_, _MODULE_, "Creando objeto en pin %x", gpio_zc);
	_level = EdgeActiveAreBoth;
    _iizc = 0;
    if(gpio_zc != NC){
        _iizc = new InterruptIn(gpio_zc);
        
        // Asocia manejadores de interrupción del pin de entrada
        _iizc->rise(NULL);
        _iizc->fall(NULL);
    }
    // Desactiva las callbacks de notificación
    _isrRiseCb = callback(&defaultNullCallback);
    _isrFallCb = callback(&defaultNullCallback);
}


//------------------------------------------------------------------------------------
void Zerocross::enableEvents(LogicLevel level, Callback<void(LogicLevel)>isrZcCb){
    if(level == EdgeActiveIsFall){
        _isrFallCb = isrZcCb;
        if(_iizc){
            _iizc->fall(callback(this, &Zerocross::isrFallCallback));
        }
        // si no hay pin, habilita el simulador
        else{
        	_tick_sim.attach_us(callback(this, &Zerocross::isrFallCallback), 20000);
        }
    }
    if(level == EdgeActiveIsRise){
        _isrRiseCb = isrZcCb;
        if(_iizc){
            _iizc->rise(callback(this, &Zerocross::isrRiseCallback));
        }
        // si no hay pin, habilita el simulador
		else{
			_tick_sim.attach_us(callback(this, &Zerocross::isrRiseCallback), 20000);
		}
    }
    if(level == EdgeActiveAreBoth){
        _isrFallCb = isrZcCb;
        _isrRiseCb = isrZcCb;
        if(_iizc){
            _iizc->fall(callback(this, &Zerocross::isrFallCallback));
            _iizc->rise(callback(this, &Zerocross::isrRiseCallback));
        }
        // si no hay pin, habilita el simulador
		else{
			_tick_sim.attach_us(callback(this, &Zerocross::isrRiseCallback), 10000);
		}

    }
}


//------------------------------------------------------------------------------------
void Zerocross::disableEvents(LogicLevel level){
    if(level == EdgeActiveIsFall){
        if(_iizc){
            _iizc->fall(NULL);
        }
        else{
        	DEBUG_TRACE_I(_EXPR_, _MODULE_, "Parando simulador");
        	_tick_sim.detach();
        }
        _isrFallCb = callback(&defaultNullCallback);
    }
    if(level == EdgeActiveIsRise){
        if(_iizc){
            _iizc->rise(NULL);
        }
        else{
        	DEBUG_TRACE_I(_EXPR_, _MODULE_, "Parando simulador");
        	_tick_sim.detach();
        }
        _isrRiseCb = callback(&defaultNullCallback);
    }
    if(level == EdgeActiveAreBoth){
        if(_iizc){
            _iizc->fall(NULL);
            _iizc->rise(NULL);
        }
        else{
        	DEBUG_TRACE_I(_EXPR_, _MODULE_, "Parando simulador");
        	_tick_sim.detach();
        }
        _isrFallCb = callback(&defaultNullCallback);
        _isrRiseCb = callback(&defaultNullCallback);
    }
}


 
//------------------------------------------------------------------------------------
//-- PRIVATE METHODS IMPLEMENTATION --------------------------------------------------
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
void Zerocross::isrRiseCallback(){
    _isrRiseCb.call(EdgeActiveIsRise);
}


//------------------------------------------------------------------------------------
void Zerocross::isrFallCallback(){
    _isrFallCb.call(EdgeActiveIsFall);
}

