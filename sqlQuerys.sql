-- RETORNA DATOS DE CADA RESERVACION
select c.Nombre, c.Apellido1, c.Apellido2, cXr.fkReserva Reservación, c.pkCliente Pasaporte, v.pkVuelo Número_vuelo, a.Tipo Tipo_boleto, a.Fila, a.Columna
	from Cliente c
		inner join ClienteXReserva cXr on c.pkCliente = cXr.fkCliente
        inner join Asiento a on cXr.fkAsiento = a.pkAsiento
        inner join Vuelo v on a.fkVuelo = v.pkVuelo;

-- OBTIENE EL MONTO TOTAL DE CADA RESERVACION
select sum(t.precio)
	from Cliente c
		inner join ClienteXReserva cXr on c.pkCliente = cXr.fkCliente
        inner join Asiento a on cXr.fkAsiento = a.pkAsiento
        inner join Vuelo v on a.fkVuelo = v.pkVuelo
        inner join Tarifa t on v.pkVuelo = t.fkVuelo
	where SUBSTRING(t.Tipo,1,1) = SUBSTRING(a.Tipo, 1, 1)
    and cXr.fkReserva = 2
		and SUBSTRING(t.Tipo, 2) = c.Edad;

-- ELIMINA LOS REGISTROS INGRESADOS DESDE C
delete from Cliente where pkCLiente != 274628 and pkCLiente != 182959;

-- OBTIENE LOS CLIENTES DE UNA RESERVACION
SELECT a.fkVuelo, c.Nombre, c.Apellido1, c.Apellido2, c.pkCliente, a.Tipo, a.Fila, a.Columna, t.Precio
				FROM Cliente c
					INNER JOIN ClienteXReserva cXr ON c.pkCliente = cXr.fkCliente
					INNER JOIN Asiento a ON cXr.fkAsiento = a.pkAsiento
					INNER JOIN Vuelo v ON a.fkVuelo = v.pkVuelo
                    INNER JOIN Tarifa t ON v.pkVuelo = t.fkVuelo
				where SUBSTRING(t.Tipo,1,1) = SUBSTRING(a.Tipo, 1, 1)
				and SUBSTRING(t.Tipo, 2) = c.Edad
                and cXr.fkReserva = 2;


-- INFO DE VUELO
SELECT v.pkVuelo, c1.Codigo Ciudad_salida, v.Salida, c2.Codigo Ciudad_destino, v.LLegada, ma.Descripcion Marca, mo.descripcion Modelo
from Vuelo v
	inner join Ciudad c1 on v.fkOrigen = c1.pkCiudad
    inner join Ciudad c2 on v.fkDestino = c2.pkCiudad
    inner join Avion a on v.fkAvion = a.pkAvion
    inner join Modelo mo on a.fkModelo = mo.pkModelo
    inner join Marca ma on mo.fkMarca = ma.pkMarca;
    where v.pkVuelo = ?;

-- Asientos por tipo en un vuelo
select concat(Tipo, Estado) Tipo, count(pkAsiento) Cantidad
from Asiento
	inner join Vuelo on fkVuelo = pkVuelo
	where Tipo <> 'BI' and Tipo <> 'SI' and Tipo <> 'EI'
	group by concat(Tipo, Estado)
    order by Tipo;

-- Total de asientos O y L
select concat(Tipo, Estado) Asiento, fila, columna
from Asiento
where Tipo <> 'BI' and Tipo <> 'SI' and Tipo <>'EI' and fkVuelo = 1 
order by fila, columna;

-- Cantidad de Adultos e Infantes por vuelo
select c.Edad, count(v.pkVuelo) Total
from Asiento a
	inner join ClienteXReserva cXr on cXr.fkAsiento = a.pkAsiento
	inner join Vuelo v on v.pkVuelo = a.fkVuelo
    inner join Cliente c on cXr.fkCliente = c.pkCliente
    group by c.Edad order by c.Edad;
    
select * from ClienteXReserva;

select r.pkReserva
	from Reserva r
		inner join Vuelo v on r.fkVuelo = v.pkVuelo;
        
        
SELECT a.fkVuelo, c.Nombre, c.Apellido1, c.Apellido2, c.pkCliente, a.Tipo, a.Fila, a.Columna, t.Precio
				FROM Cliente c
					INNER JOIN ClienteXReserva cXr ON c.pkCliente = cXr.fkCliente
					INNER JOIN Asiento a ON cXr.fkAsiento = a.pkAsiento
					INNER JOIN Vuelo v ON a.fkVuelo = v.pkVuelo
                    INNER JOIN Tarifa t ON v.pkVuelo = t.fkVuelo
                    INNER JOIN Reserva r ON cXr.fkReserva = r.pkReserva and v.pkVuelo = r.fkVuelo;
				where SUBSTRING(t.Tipo,1,1) = a.Tipo
				and SUBSTRING(t.Tipo, 2) = c.Edad and a.Tipo = 'I'
                and cXr.fkReserva = 2;
select * from Asiento;

SELECT a.fkVuelo, c.Nombre, c.Apellido1, c.Apellido2, c.pkCliente, a.Tipo, a.Fila, a.Columna, t.Precio
				FROM Cliente c
					INNER JOIN ClienteXReserva cXr ON c.pkCliente = cXr.fkCliente
					INNER JOIN Asiento a ON cXr.fkAsiento = a.pkAsiento
					INNER JOIN Vuelo v ON a.fkVuelo = v.pkVuelo
                    INNER JOIN Tarifa t ON v.pkVuelo = t.fkVuelo
				where SUBSTRING(t.Tipo,1,1) = SUBSTRING(a.Tipo, 1, 1)
				and SUBSTRING(t.Tipo, 2) = c.Edad
                and cXr.fkReserva = 2;