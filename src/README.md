# Com funciona?

Hi ha una funcio en cpp que literalment agafa els tickes de etoro guardats a input/ i busca amb la url a yahoo finance el ticker de yahoo que correspongui al nom que diu etoro.

Això dona molts errors de "not found" ja que a vegades els noms que surten a yahoo finance són lleugerament diferents als de etoro, i això fa que yahoo no ho trobi, encara que existeix.

## Proposta de millora:

S'hauria de refinar el programa perquè si retorna not found, torni a intentar-ho escurçant el nom de la acció, a veure si així yahoo ho troba.