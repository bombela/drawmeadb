<script setup lang="ts">
import Tile from './components/Tile.vue'
import GraphRender from './components/GraphRender.vue'
import SQL from './components/SQL.vue'
import Spinner from './components/Spinner.vue'

import { reactive, computed, onMounted, watch } from 'vue'

import axios from "axios"

interface State {
  solvedID?: string,
  assignment: string,
  updating: boolean,
  error?: string,
  last_submission_succeeded: boolean,
}

const state: State = reactive({
  solvedID: undefined,
  assignment: "",
  updating: true,
  error: undefined,
  last_submission_succeeded: false,
})

const solved_id_re = new RegExp("/play/([a-zA-Z0-9]{15})/?");
onMounted(() => {
	let match = location.pathname.match(solved_id_re);
	if (match) {
		state.solvedID = match[1];
		axios.get(`__PLAY_URL__/${state.solvedID}/assignment.txt`).then((response) => {
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
		let name: string = (source.target as HTMLButtonElement).name;
		axios.get(import.meta.env.BASE_URL + `/${name}.txt`).then((response) => {
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
	state.last_submission_succeeded = false;
	axios.post("__PLAY_URL__/solve", state.assignment).then((response) => {
		let r = response.data;
		if (r.Solved) {
			state.solvedID = r.Solved;
			history.pushState({}, "", "/play/" + state.solvedID);
			state.last_submission_succeeded = true;
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
	  <div class="header">
		  <img alt="Draw me a db logo" class="logo" src="./assets/logo.svg" width="96" height="96" />
		  <div>Draw me a db</div>
	  </div>
  </header>

  <main>
	<Tile title="Assignment">
		<div>
			Examples:
			<a @click="fetch_example" name="ex1" href="javascript:void(0)">Movie theater</a>,
			<a @click="fetch_example" name="ex2" href="javascript:void(0)">Bookstore</a>,
			<a @click="fetch_example" name="ex3" href="javascript:void(0)">UPS</a>.
		</div>
		<textarea class="maximized assignment" v-model.trim=state.assignment :disabled=is_submit_disabled placeholder="Describe here the entities and relation you want. Load some examples with the link above."></textarea>
		<div class="submit_bar">
			<div v-if=state.error class="error">Failed: {{ state.error }}</div>
			<div v-if=state.last_submission_succeeded class="info">Try again for a different answer.</div>
			
			<button class="submit_assignment" @click="submit_assignment" :disabled="is_submit_disabled">
				<Spinner v-if=state.updating></Spinner>
				<span v-else>Draw me a db</span>
			</button>
		</div>
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
  line-height: 1em;
}

main {
	display: grid;
	grid-template-columns: repeat(auto-fit, minmax(550px, 1fr));
	justify-items: center;
}

div.header {
	display: flex;
	justify-content: center;
	margin: auto 0;
	font-size: 5em;
	place-items: center;
}

img.logo {
	margin-right: 0.5em;
}

textarea.assignment {
	font-family: helvetica;
}

div.error {
	color: var(--color-error);
	padding-right: 2em;
}

div.info {
	color: var(--color-action2);
	padding-right: 2em;
}

div.submit_bar {
	display: flex;
	justify-content: flex-end;
	align-items: center;
}

button.submit_assignment {
	appearance: button;
	backface-visibility: hidden;
	background-color: var(--color-action1);
	border-radius: 6px;
	border-width: 0;
	box-shadow: rgba(50, 50, 93, .1) 0 0 0 1px inset,rgba(50, 50, 93, .1) 0 2px 5px 0,rgba(0, 0, 0, .07) 0 1px 1px 0;
	box-sizing: border-box;
	color: #fff;
	cursor: pointer;
	font-size: 100%;
	height: 44px;
	line-height: 1.15;
	margin: 12px 0 0;
	outline: none;
	overflow: hidden;
	padding: 0 25px;
	text-align: center;
	text-transform: none;
	transform: translateZ(0);
	transition: all .2s,box-shadow .08s ease-in;
	user-select: none;
	-webkit-user-select: none;
	touch-action: manipulation;
	width: 30%;
}

button.submit_assignment:disabled {
  cursor: default;
  background-color: var(--color-action-disabled);
}

button.submit_assignment:hover:enabled {
  background-color: var(--color-action2);
}

button.submit_assignment:focus:enabled {
  box-shadow: rgba(50, 50, 93, .1) 0 0 0 1px inset, rgba(50, 50, 93, .2) 0 6px 15px 0, rgba(0, 0, 0, .1) 0 2px 2px 0, rgba(50, 151, 211, .3) 0 0 0 4px;
}
</style>
