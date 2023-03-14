<script setup lang="ts">
import { computed, reactive, watch } from 'vue'

import axios from "axios"

const props = defineProps<{
	solvedID?: string,
}>()

interface State {
	schema?: string,
}

const state: State = reactive({
	schema: undefined,
})


async function fetch_schema() {
}

watch(() => props.solvedID, async (solvedID) => {
	if (solvedID !== undefined) {
		let response = await axios.get(__PLAY_URL__ + solvedID + "/schema.txt");
		state.schema = response.data;
	}
})

</script>

<template>
		<textarea v-if="solvedID === undefined" class="maximized" disabled readonly>CREATE TABLE ...</textarea>
		<textarea v-else class="maximized" readonly>{{ state.schema }}</textarea>
</template>

<style scoped>
</style>

