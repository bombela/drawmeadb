<script setup lang="ts">
import HelloWorld from './components/HelloWorld.vue'
import TheWelcome from './components/TheWelcome.vue'
import Tile from './components/Tile.vue'
import GraphRender from './components/GraphRender.vue'
</script>

<template>
  <header>
	<img alt="Draw me a db logo" class="logo" src="./assets/logo.svg" width="32" height="32" />

    <div class="wrapper">
		<div>Draw me a db</div>
    </div>
  </header>

  <main>
	<Tile title="Assignment">
		<div>
			Example:
			<a>Movie theater</a>
			<a>Another one</a>
			<a>One more</a>
		</div>
		<textarea  class="maximized">Assignment: Conceptual Modeling of a Movie Rental System

Description: You are tasked with modeling a movie rental system for a local video store. The system should keep track of customers, movies, rentals, and late fees.

Entities:

Customer: A person who rents movies from the store. Each customer has a unique customer ID, name, address, phone number, and email address.
Movie: A movie that can be rented from the store. Each movie has a unique movie ID, title, release year, genre, director, and length.
Rental: A rental is a transaction where a customer rents a movie for a specified period of time. Each rental has a unique rental ID, a rental date, a due date, and a return date.
Late Fee: A late fee is a charge that is applied to a rental if the movie is returned after the due date. Each late fee has a unique late fee ID, an amount, and a date.

Relations:

A customer can rent many movies, but a movie can only be rented by one customer at a time.
A rental is associated with one customer and one movie.
A movie can have many late fees, and a late fee is associated with only one movie.</textarea>
	<button>Draw me a db</button>
	</Tile>
	<Tile title="Conceptual model">
		<GraphRender placeholder_img="/placeholder_conceptual.png" />
	</Tile>
	<Tile title="Physical model">
		<GraphRender placeholder_img="placeholder_physical.png" />
	</Tile>
	<Tile title="SQL Schema">
		<textarea class="maximized">CREATE TABLE ...</textarea>
	</Tile>
  </main>
</template>

<style scoped>
header {
  line-height: 1.5;
}

.logo {
  display: block;
  margin: 0 auto 0rem;
}

.tile-row {
  display: flex;
  flex-direction: row;
  justify-content: space-evenly;
}

/*
@media (min-width: 800px) {
  header {
    display: flex;
    place-items: center;
    padding-right: calc(var(--section-gap) / 2);
  }

  .logo {
    margin: 0 2rem 0 0;
  }

  header .wrapper {
    display: flex;
    place-items: flex-start;
    flex-wrap: wrap;
  }
}
*/
</style>
