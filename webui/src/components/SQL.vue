<script setup lang="ts">
import { computed, reactive, watch } from 'vue'

import axios from "axios"

const props = defineProps<{
	solvedID?: string,
}>()

interface State {
	schema?: string,
	error?: string,
}

const state: State = reactive({
	schema: undefined,
	error: undefined,
})

watch(() => props.solvedID, async (solvedID) => {
	if (solvedID !== undefined) {
		try {
			let response = await axios.get(__PLAY_URL__ + `/${solvedID}/schema.txt`);
			state.schema = response.data;
			state.error = undefined;
		} catch (e: any) {
			state.error = e;
		}
	}
})

</script>

<template>
		<textarea v-if="solvedID === undefined" class="maximized" disabled readonly>CREATE TABLE ...</textarea>
		<textarea v-else-if="state.error !== undefined" class="maximized error"
		disabled readonly>{{ state.error }}</textarea>
		<textarea v-else class="maximized" eadonly>{{ state.schema }}</textarea>
</template>

<style scoped>
</style>
