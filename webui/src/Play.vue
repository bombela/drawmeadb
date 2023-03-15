<script setup lang="ts">
import Tile from './components/Tile.vue'
import GraphRender from './components/GraphRender.vue'
import SQL from './components/SQL.vue'

import { reactive, computed, onMounted, watch } from 'vue'

import axios from "axios"

interface State {
  solvedID?: string,
  assignment: string,
  updating: boolean,
  error?: string,
}

const state: State = reactive({
  solvedID: undefined,
  assignment: "",
  updating: true,
  error: undefined,
})

const solved_id_re = new RegExp("/play/([a-zA-Z0-9]{15})/?");
onMounted(() => {
	let match = location.pathname.match(solved_id_re);
	if (match) {
		state.solvedID = match[1];
		axios.get(`${__PLAY_URL__}${state.solvedID}/assignment.txt`).then((response) => {
			state.assignment = response.data;
		}).finally(() => {
			state.updating = false;
		});
	} else {
		state.updating = false;
	}
})

function fetch_example(source: MouseEvent) {
	state.updating = true;
	if (source.target) {
		let name: string = source.target.name;
		axios.get(`/${name}.txt`).then((response) => {
			state.assignment = response.data;
		}).finally(() => {
			state.updating = false;
		});
	}
	return true;
}

function submit_assignment() {
	state.updating = true;
	state.error = undefined;
	axios.post(__PLAY_URL__ + "solve", state.assignment).then((response) => {
		let r = response.data;
		if (r.Solved) {
			state.solvedID = r.Solved;
			history.pushState({}, "", "/play/" + state.solvedID);
		} else {
			state.error = r.Error;
		}
	}).finally(() => {
		state.updating = false;
	});
}

const is_submit_disabled = computed(() => {
	return state.updating || state.assignment.length == 0;
});
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
			Examples:
			<a @click="fetch_example" name="ex1" href="javascript:void(0)">Movie theater</a>,
			<a @click="fetch_example" name="ex2" href="javascript:void(0)">Bookstore</a>,
			<a @click="fetch_example" name="ex3" href="javascript:void(0)">Fitness tracker</a>.
		</div>
		<textarea class="maximized" v-model.trim=state.assignment placeholder="Describe here the entities and relation you want. Load some examples with the link above."></textarea>
	<button @click="submit_assignment" :disabled="is_submit_disabled">Draw me a db</button>
	<span v-if=state.error class="error">Failed: {{ state.error }}</span>
	</Tile>
	<Tile title="Conceptual model">
		<GraphRender kind="conceptual" :solvedID=state.solvedID />
	</Tile>
	<Tile title="Physical model">
		<GraphRender kind="physical" :solvedID=state.solvedID />
	</Tile>
	<Tile title="SQL Schema">
		<SQL :solvedID=state.solvedID />
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

span.error {
	color: red;
	padding-left: 1em;
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
