use gestionVuelos;

insert into Marca(Descripcion)
values ('BOEING');

insert into Marca(Descripcion)
values ('AIRBUS');

insert into Marca(Descripcion)
values ('EMBRAER');

-- Marcas

insert into Modelo(descripcion, fkMarca)
values ('6D', 1);

insert into Modelo(descripcion, fkMarca)
values ('7J7', 1);

insert into Modelo(descripcion, fkMarca)
values ('GA-1', 1);

insert into Modelo(descripcion, fkMarca)
values ('737 Classic', 1);

insert into Modelo(descripcion, fkMarca)
values ('NC-135', 1);

--   BOEING modelos

insert into Modelo(descripcion, fkMarca)
values ('A300', 2);

insert into Modelo(descripcion, fkMarca)
values ('Beluga XL', 2);

insert into Modelo(descripcion, fkMarca)
values ('A340', 2);

insert into Modelo(descripcion, fkMarca)
values ('A330neo', 2);

insert into Modelo(descripcion, fkMarca)
values ('NSR', 2);

-- AIRBUS modelos


insert into Modelo(descripcion, fkMarca)
values ('EMB 110 Bandeirante', 3);

insert into Modelo(descripcion, fkMarca)
values ('EMB 120 Brasilia', 3);

insert into Modelo(descripcion, fkMarca)
values ('EMB 121 Xingú', 3);

insert into Modelo(descripcion, fkMarca)
values ('FMA CBA 123 Vector', 3);

insert into Modelo(descripcion, fkMarca)
values ('ERJ 145', 3);

-- EMBRAER modelos


