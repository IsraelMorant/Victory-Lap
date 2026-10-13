// --- Pines de Salida hacia el Driver del Motor ---
const int pinIN1 = 23; // Control de dirección 1
const int pinIN2 = 22; // Control de dirección 2
const int pinENA = 21; // Control de Velocidad (PWM) - Conectar al Enable del driver

// --- Pines de Entrada (DIP Switch) ---
// Conectaremos los pines 1, 2 y 3 de un lado del DIP switch a estos pines,
// y el lado opuesto del DIP switch irá conectado a GND (Tierra).
const int swLento = 15; // Interruptor 1
const int swMedio = 4;  // Interruptor 2
const int swRapido = 5; // Interruptor 3

// --- Variables de Estado ---
int velocidadActual = -1; // -1 para forzar la primera actualización
const int VEL_LENTA = 100;
const int VEL_MEDIA = 180;
const int VEL_RAPIDA = 255;

void setup() {
  Serial.begin(115200);
  
  // Configurar pines del motor como salida
  pinMode(pinIN1, OUTPUT);
  pinMode(pinIN2, OUTPUT);
  pinMode(pinENA, OUTPUT);

  // Configurar pines de interruptores como entradas con pull-up interno
  // Cuando el interruptor esté en "ON", conectará el pin a GND (leerá LOW)
  // Cuando esté en "OFF", estará abierto (leerá HIGH)
  pinMode(swLento, INPUT_PULLUP);
  pinMode(swMedio, INPUT_PULLUP);
  pinMode(swRapido, INPUT_PULLUP);

  // Iniciar con el motor detenido
  DetenerMotor();
  Serial.println("Sistema iniciado. Mueve un interruptor del DIP switch (1 a 3) a la posición ON.");
}

void loop() {
  int nuevaVelocidad = 0;
  String nombreVelocidad = "DETENIDO";

  // Leemos el estado de los interruptores (LOW = ON, HIGH = OFF)
  // Usamos una estructura if-else para dar prioridad si se encienden varios por error.
  if (digitalRead(swRapido) == LOW) {
    nuevaVelocidad = VEL_RAPIDA;
    nombreVelocidad = "ALTA";
  } else if (digitalRead(swMedio) == LOW) {
    nuevaVelocidad = VEL_MEDIA;
    nombreVelocidad = "MEDIA";
  } else if (digitalRead(swLento) == LOW) {
    nuevaVelocidad = VEL_LENTA;
    nombreVelocidad = "BAJA";
  } else {
    // Si todos están en HIGH (OFF)
    nuevaVelocidad = 0;
  }

  // Comparamos para no estar enviando la misma señal constantemente
  if (nuevaVelocidad != velocidadActual) {
    if (nuevaVelocidad == 0) {
      DetenerMotor();
      Serial.println("Motor Detenido (Interruptores 1-3 en OFF)");
    } else {
      cambiarVelocidad(nuevaVelocidad, nombreVelocidad);
    }
  }

  delay(50); // Pequeña pausa por estabilidad de lectura
}

// Función para cambiar la velocidad y asegurar el giro
void cambiarVelocidad(int nuevaVelocidad, String nombreVelocidad) {
  velocidadActual = nuevaVelocidad;
  
  // Establecemos el sentido de giro (ej. siempre horario para esta prueba)
  digitalWrite(pinIN1, HIGH);
  digitalWrite(pinIN2, LOW);
  
  // Aplicamos la velocidad mediante PWM al pin ENA
  analogWrite(pinENA, velocidadActual);
  
  Serial.print("Cambiando velocidad a: ");
  Serial.print(nombreVelocidad);
  Serial.print(" (PWM: ");
  Serial.print(velocidadActual);
  Serial.println(")");
}

// Función para detener el motor completamente
void DetenerMotor() {
  velocidadActual = 0;
  analogWrite(pinENA, 0); // Apagar PWM
  digitalWrite(pinIN1, LOW);
  digitalWrite(pinIN2, LOW);
}