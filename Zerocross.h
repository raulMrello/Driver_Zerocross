/*
 * Zerocross.h
 *
 *  Created on: Sep 2017
 *      Author: raulMrello
 *
 *	Zerocross es el módulo encargado de gestionar los eventos ocurridos en la entrada conectada al detector de paso por cero.
 *  Se podrá configurar para generar eventos en el flanco de bajada, de subida o en ambos. Las notificaciones se realizarán
 *  mediante la instalación de callbacks dedicadas. Dichas callbacks se ejecutarán en el contexto ISR
 */
 
#ifndef __Zerocross__H
#define __Zerocross__H

#include "mbed.h"

   
class Zerocross{
  public:
    
    /** Tipo de flanco activo*/
    enum LogicLevel{
        EdgeActiveIsFall,   /// Activo sólo en flanco de bajada
        EdgeActiveIsRise,   /// Activo sólo en flanco de subida
        EdgeActiveAreBoth   /// Activo en ambos flancos
    };
    
    
	/** Constructor 
     *  Crea una entrada InterruptIn asociada un pin gpio. En el caso de no utilizar el pin (NC) se habilita
     *  un simulador de eventos temporizado cada 10ms o 20ms dependiendo del LogicLevel seleccionado
     *  @param gpio_zc Pin de entrada asociado al zerocross
     *  @param defdbg Flag para activar depuración
     */
    Zerocross(PinName gpio_zc, bool defdbg=false);
    
    
    /** Destructor */
    ~Zerocross();
  
  
	/** enableEvents
     *  Instala callback para procesar los eventos de paso por cero dependiendo del nivel
     *  @param level Nivel a activar
     *  @param pressCb Callback a instalar
     */
    void enableEvents(LogicLevel level, Callback<void(LogicLevel)>isrZcCb);
  
    
	/** disableEvents
     *  Desactiva la notificación de eventos de un nivel concreto. De esta forma se permite desactivar eventos
     *  de un flanco, pero dejar otro flanco activo.
     *  @param level Nivel a desactivar
     */
    void disableEvents(LogicLevel level);
         
  private:
  
    LogicLevel _level;                          /// Nivel lógico configurado
    InterruptIn* _iizc;                         /// Entrada de interrupción asociada al GPIO
    Callback<void(LogicLevel)> _isrRiseCb;      /// Callback para notificar eventos en flanco de subida (ISR)
    Callback<void(LogicLevel)> _isrFallCb;      /// Callback para notificar eventos en flanco de bajada (ISR)
    Ticker _tick_sim;							/// Ticker para la simulación eventos
    bool _defdbg;								/// Flag para depuración
    
	/** isrRiseCallback
     *  ISR para procesar eventos de cambio a nivel alto
     */
    void isrRiseCallback();
    
	/** isrFallCallback
     *  ISR para procesar eventos de cambio a nivel bajo
     */
    void isrFallCallback();  
  
};



#endif /*__Zerocross__H */

/**** END OF FILE ****/


